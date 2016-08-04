
import json
f = open("dump.dat", "r")
i = 0

min_num = 700000000
max_num = 0
count = 0
for line in f.readlines():
    js = json.loads(line)
    signs = js["signatures"]
    for sign in signs:
        count = count + 1
        length = len(sign)
        if length < min_num:
            min_num = length
        if length > max_num:
            max_num = length
        if length == 0:
            print js["name"], " has signatures that has a length of 0"
        if length > 10000:
            print js["name"], "has signatures that larger than 10000, which is", length
print "max: ", max_num
print "min: " , min_num
print "count", count
f.close()
