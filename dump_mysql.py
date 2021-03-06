import MySQLdb

f = open("dump.dat", "w")
mysql = MySQLdb.connect(user="root", passwd="123456", host="166.111.82.26", db="Artemis-web", charset="utf8")
cursor = mysql.cursor()
#sql = "SELECT id, signatures FROM components WHERE coverity_scan != 'null' AND signatures IS NOT NULL or id = 12663 or id =12607"
sql = "SELECT signatures.id, signatures FROM `signatures` join `components` where (components.id = signatures.id and components.CPE_RESULT = 1 and components.status > 0 and signatures != '[\"\"]') or components.id = signatures.id and signatures.id = 12663"
cursor.execute(sql)
for line in cursor:
    string = "{ \"name\": %s, \"signatures\": %s}\n" % (str(line[0]), str(line[1]))
    f.write(string)
    f.flush()
f.close()
