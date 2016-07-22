# -*- coding:utf-8 -*-
import sys
import MySQLdb
import os
import json
import commands
import binascii
import esm
import time
class Timer:
    def __init__(self):
        self.start = time.time()
    def restart(self):
        self.start = time.time()

    def get_time_hhmmss(self):
        end = time.time()
        m, s = divmod(end - self.start, 60)
        h, m = divmod(m, 60)
        timer_str = "%02d:%02d:%02d" % (h, m, s)
        return timer_str
def memo(f):
    cache = {}
    def memorized(x):
        if not x in cache:
            cache[x] = f(x)
        return cache[x]
    return memorized

def openfile_tohex(file_name):
    f = open(file_name,'r')
    hex_string = binascii.b2a_hex(f.read())
    f.close()
    return hex_string

@DeprecationWarning
def make_finder(signatures):
    idx = esm.Index()
    for signature in signatures:
        if len(signature) > 0:
            idx.enter(signature)
    idx.fix()

    def finder(string):
        return len(set(h for _,h in idx.query(string)))
    return finder

class Finder:
    def __init__(self, signatures):
        self.index = esm.Index()
        for signature in signatures:
            if len(signature) > 0:
                self.index.enter(signature)
        self.index.fix()
    def __call__(self, x):
        return x[0], len(set(h for _,h in self.index.query(x[1])))
def get_executables(dir_):
    fileList = []
    for root, _, files in os.walk(dir_):
        for f_name in files:
            full_path = os.path.join(root, f_name)
            exInfo = commands.getoutput("file -b " + full_path)
            if ('ELF' in exInfo) or (('PE' in exInfo) and ('executable' in exInfo)) or ('Mach-O' in exInfo):
                fileList.append(full_path)
    return fileList

class DAO:
    def __init__(self, ip, port_num, username, password, db_name):
        self.conn =MySQLdb.connect(host= ip, \
        user= username, passwd= password, \
        db=db_name,port= port_num,charset='utf8')
        self.cursor = self.conn.cursor()

    def executeQuery(self,sql):
        self.cursor.execute(sql)
        return self.cursor

    def get_signatures(self, identity):
        sql = "SELECT signatures FROM components WHERE id = " + str(identity) + ";"
        result = self.executeQuery(sql).fetchone()
        return json.loads(result[0])

    def close(self):
        self.cursor.close()
        self.conn.close()

def main():
    timer = Timer()
    dir_path = sys.argv[1]
    dao = DAO("166.111.82.26", 3306, "root", "123456", "Artemis")
    print "Connected to the database"
    print "Selecting components ..."
    components = [i[0]for i in dao.executeQuery("SELECT id FROM components WHERE coverity_scan != \'null\' AND signatures IS NOT NULL;")]
    print "Finish selecting components"
    print "Start Scanning the directionary ..."
    executables = map(lambda x: (x, openfile_tohex(x)), get_executables(dir_path))
    print "Finish scanning the directionary"
    print "Start scanning:"
    for comp in components:
        signatures = dao.get_signatures(comp)
        signature_num = len(signatures)
        finder = Finder(signatures)
        print "ID: ", str(comp)
        print "Total: ", str(signature_num)
        print "-------"
        results = map(finder, executables)

        for name, hit in results:
            percent = hit / float(signature_num)
            if hit > 5 and percent > 0.01:
                print "File: ", name
                print "Hits: ", str(hit)
                print "Percentage: ", str(percent * 100), "%"
                print "-------"
        print "-----------------------------------------------------------------"
    dao.close()
    print timer.get_time_hhmmss()
if __name__ == "__main__":
    main()
