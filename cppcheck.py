#!/usr/bin/env python

import subprocess


STANDARD = 'c++11'


# Extraire les #define du compilateur

command = 'gcc --std={} -x c++ -dM -E -'.format(STANDARD).split()
with open('/dev/null') as input_f:
    output = subprocess.check_output(command, stdin=input_f)

defines = {}
for line in output.splitlines():
    parts = line.split(None, 2)
    
    if len(parts) != 3:
        continue
    if parts[0] != '#define':
        continue
    
    name, value = parts[1:]
    defines[name] = value

# Executer cppcheck

command = 'cppcheck --check-config --std={} --platform=unix64 --enable=all'.format(STANDARD).split()
command += ['-D{}={}'.format(*item) for item in defines.iteritems()]
command += ['-I', './include', '-I', '/opt/local/include']
command += ['./src', './include']

with open('/dev/null') as input_f:
    subprocess.check_call(command, stdin=input_f)
