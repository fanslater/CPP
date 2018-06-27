import sys
import string
import re

f = open(r'version.h', 'r+')
lines = f.readlines()
data = []
for line in lines:
        tmp = re.split('\s+', line)
        if len(tmp) > 2 and tmp[1] == 'RELEASE_VER_SUB2':
            tmp[2] = str(int(tmp[2])+1);
            strtmp = ' '.join(tmp)
            strtmp += '\n'
            data.append(strtmp)
            print "###"+strtmp
        else:
            data.append(line)
f.seek(0)
f.writelines(data)
f.close()