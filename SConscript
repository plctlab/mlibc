from building import *
Import('rtconfig')

cwd = GetCurrentDir()
src = Glob('src/*.c')

CPPPATH = [cwd + '/include']
CPPDEFINES = ['RT_USING_MLIBC', 'RT_USING_LIBC']

LIBS = ['gcc']
CCFLAGS   = "-nostdinc"
LINKFLAGS = " -nostdlib"

AddDepend(['RT_USING_MLIBC', 'RT_USING_LIBC'])

group = DefineGroup('Compiler', src, depend = ['PKG_USING_MLIBC'], CPPPATH = CPPPATH, LINKFLAGS = LINKFLAGS, CPPDEFINES = CPPDEFINES, LIBS = LIBS, CCFLAGS=CCFLAGS)

Return('group')
