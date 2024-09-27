from building import *
import os
Import('rtconfig')

cwd = GetCurrentDir()
src = Glob('src/*.c')
objs = []
list = os.listdir(cwd)

CPPPATH = [cwd + '/include']
CPPPATH += [cwd + '/src/internal']
CPPDEFINES = ['RT_USING_MLIBC', 'RT_USING_LIBC']

LIBS = ['gcc']
CCFLAGS   = ""
LINKFLAGS = " -nostartfiles -nostdlib -nostdinc -lgcc "

if GetDepend('PKG_USING_MLIBC'):
    AddDepend(['RT_USING_MLIBC', 'RT_USING_LIBC'])
    group = DefineGroup('Compiler', src, depend = ['PKG_USING_MLIBC'], CPPPATH = CPPPATH, LINKFLAGS = LINKFLAGS, CPPDEFINES = CPPDEFINES, LIBS = LIBS, CCFLAGS=CCFLAGS)

    objs += group
    for d in list:
        path = os.path.join(cwd, d)
        if os.path.isfile(os.path.join(path, 'SConscript')):
            objs = objs + SConscript(os.path.join(d, 'SConscript'))

Return('objs')