import os
import sys
import pefile
import esm

'''
tuning this parameter to get a better curracy
'''

COMMON = set([
'rand', 'malloc', 'realloc', 'memset', 'exit', 'free', 'calloc', 'memcpy', 'memmove',
 'GetVersion', 'printf', 'strchr', 'strncmp', 'fread',
 'fclose', 'fprintf', 'sprintf', '_snprintf','fopen', 'strncpy', 'fseek', 'fwrite',
 'strlen', 'strcmp', 'memchr', 'ferror',
 'GetCurrentProcess', 'TerminateProcess', 'SetUnhandledExceptionFilter', 'GetCurrentProcessId',
  'GetTickCount', 'InterlockedExchange', 'QueryPerformanceCounter', 'UnhandledExceptionFilter',
  'IsDebuggerPresent', 'MultiByteToWideChar', 'GetSystemTimeAsFileTime', 'Sleep', 'GetCurrentThreadId',
 'memcmp', '_lock', '_unlock'
 ])

def parse_index(absPath):
    print "file: " + absPath
    pe_file = pefile.PE(absPath)


    index = esm.Index()
    i = 0

    for entry in pe_file.DIRECTORY_ENTRY_IMPORT:
        #print entry.dll
        for imp in entry.imports:
            #print '\t', imp.name
            name = imp.name
            if(name and not name in COMMON):
                i = i + 1
                index.enter(name)
    index.fix()
    return i, index

'''
This function can count the match in a text file
'''
def ac_match(filename, index):
    with open(filename,'r') as f:
        input_text = f.read()
    hits = index.query(input_text)
    matches = []
    for rng, match in hits:
        matches.append(match)
    return matches

def getfiles(root_dir):
    fs = []
    for root, _, files in os.walk(root_dir):
        for fls in files:
            fs.append(root + '/' + fls)
    return fs

def main():
    binary_name = sys.argv[1]
    folder_name = sys.argv[2]

    files = getfiles(folder_name)
    i, index = parse_index(binary_name)
    matches = []
    for filename in files:
        mtchs = ac_match(filename, index)
        #print str(match_num) + " matches in " + filename
        matches = matches + mtchs
    matches = set(list(matches))
    print "Number of signatures: " + str(i)

    print "Totally " + str(len(matches)) + " matches."

    #print str(len(matches) * 100 / long(i)) + "%" + " match"
    print matches

if __name__ == "__main__":
    main()
