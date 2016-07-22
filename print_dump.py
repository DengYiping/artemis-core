
import json
f = open("dump.dat", "r")
line = str(f.readline())
print json.loads(line)["name"]
print json.loads(line)["signatures"]
