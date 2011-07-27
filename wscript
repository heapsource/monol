srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')
  opt.tool_options('compiler_cc')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('compiler_cc')
  conf.check_tool('node_addon')
  conf.check_cfg(package='mono-2', args='--libs --cflags', uselib_store='MONO')
  conf.check(header_name="mono/jit/jit.h")

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'monol'
  obj.source = """
	src/monol.cc
	src/MonolAppDomain.cc
	src/MonolAssembly.cc
	src/MonolType.cc
	src/MonolObject.cc
"""
  obj.uselib = 'MONO'