#!/usr/bin/env python

import os
import tempfile
import subprocess
from contextlib import closing


STANDARD = 'c++11'
PLATFORM = 'unix64'

GCC_BASE_COMMAND = 'gcc --std={} -x c++'.format(STANDARD).split()

cppcheck_comman_template = 'cppcheck --check-config --std=posix --std=c99 --std={} --platform={} --enable=all'
CPPCHECK_BASE_COMMAND = cppcheck_comman_template.format(STANDARD, PLATFORM).split()


# Fonctions

def get_compiler_defines():
    """Extraire les #define du compilateur"""
    command = GCC_BASE_COMMAND + '-E -dM -'.split()
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
    
    return defines


def get_compiler_includes():
    """Extraire les includes du compilateur"""
    command = GCC_BASE_COMMAND + '-E -v -'.split()
    with open('/dev/null') as input_f:
        output = subprocess.check_output(command, stdin=input_f, stderr=subprocess.STDOUT)

    includes = []
    in_includes = False
    for line in output.splitlines():
        if line == "#include <...> search starts here:":
            in_includes = True
            continue
        
        if in_includes:
            if line.startswith("End of search list"):
                in_includes = False
            else:
                includes.append(line.strip())

    return includes


def build_file_list(f):
    for basedir, dirnames, filenames in os.walk('.'):
        for filename in filenames:
            basename, ext = os.path.splitext(filename)
            ext = ext.lstrip('.')
            if ext not in ('c', 'h', 'cpp', 'hpp'):
                break
            f.write(os.path.join(basedir, filename) + '\n')


# Executer cppcheck

command = list(CPPCHECK_BASE_COMMAND)
command += ['-D{}={}'.format(*item) for item in get_compiler_defines().iteritems()]
command += ['-I', './include', '-I', '/opt/local/include']

for include in get_compiler_includes():
    command += ['-I', include]

command += ['--file-list=-']

with tempfile.TemporaryFile() as temp_f:
    build_file_list(temp_f)
    temp_f.seek(0, os.SEEK_SET)
    subprocess.check_call(command, stdin=temp_f)