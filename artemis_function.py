import redis
import os
import commands
import json
import shutil

def scan(id_, path, addr):
    """Scan a particular path and return the result as JSON in redis
    """
    #List all the files
    print "start scanning files..."
    file_list = []
    for root, _, files in os.walk(path):
        for f_name in files:
            full_path = os.path.join(root, f_name)
            exInfo = commands.getoutput("file -b \"" + full_path + "\"")
            if ('ELF' in exInfo) or (('PE' in exInfo) and ('executable' in exInfo)) or ('Mach-O' in exInfo):
                file_list.append(full_path)


    result_json = ""
    if(len(file_list) > 0):
        #Extract constant strings from file

        for line in file_list:
            result = os.popen("strings -10 \"" + line + "\"").read()
            with open(line + ".strings", "w") as f:
                f.write(result)

        #store files to a file list
        with open("list", "w") as list_f:
            for file_path in map(lambda x: x + ".strings", file_list):
                list_f.write(file_path + "\n")

        #execute C++ program and store output to result.json
        cmd = "./artemis > result.json"
        os.system(cmd)

        #read result.json
        load = lambda x: json.loads(x.replace(".strings", ""))
        with open("result.json", "r") as outfile:
            result_json =  str(  json.dumps( map( load, outfile.readlines() ) )  )

        #clean up
        os.remove("result.json")
        shutil.rmtree(path, ignore_errors=True)

        #store result to redis
        redis_conn = redis.StrictRedis(host=addr, port=6379, db=0)
        redis_conn.set(id_, result_json)
        return "success"
