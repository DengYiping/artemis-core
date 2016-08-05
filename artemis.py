import os
import commands
import sys
import json
import shutil

dir_ = sys.argv[1]
file_list = []
for root, _, files in os.walk(dir_):
    for f_name in files:
        full_path = os.path.join(root, f_name)
        exInfo = commands.getoutput("file -b \"" + full_path + "\"")
        if ('ELF' in exInfo) or (('PE' in exInfo) and ('executable' in exInfo)) or ('Mach-O' in exInfo):
            file_list.append(full_path)

if(len(file_list) > 0):
    for line in file_list:
        result = os.popen("strings -10 \"" + line + "\"").read()
        with open(line + ".strings", "w") as f:
            f.write(result)



    with open("list", "w") as list_f:
        for file_path in map(lambda x: x + ".strings", file_list):
            list_f.write(file_path + "\n")


    cmd = "/home/jeffrey/artemis/artemis-core/artemis > result.json"
    os.system(cmd)

    with open("result.json", "r") as outfile:
        print json.dumps( map( json.loads, outfile.readlines() ) )

    os.remove("result.json")
#shutil.rmtree(dir_, ignore_errors=True)
