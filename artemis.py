import os
import commands
import sys
import json
f = open("list", 'w')
dir_ = sys.argv[1]
for root, _, files in os.walk(dir_):
    for f_name in files:
        full_path = os.path.join(root, f_name)
        exInfo = commands.getoutput("file -b " + full_path)
        if ('ELF' in exInfo) or (('PE' in exInfo) and ('executable' in exInfo)) or ('Mach-O' in exInfo):
            f.write(full_path + "\n")
f.close()

cmd = "./artemis > result.json"
os.system(cmd)

with open("result.json", "r") as outfile:
    print json.dumps( map( json.loads, outfile.readlines() ) )

os.remove("result.json")
