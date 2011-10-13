import glob
import os
import platform

def RunGyp(current_file, relative_gyp_path, root_gyp_file):
  os.chdir(os.path.dirname(os.path.realpath(current_file)))
  os.system('python %s %s' % (os.path.join(relative_gyp_path, 'gyp/gyp'), root_gyp_file))

def RunTests(current_file, relative_src_path):
  os.chdir(os.path.join(os.path.dirname(os.path.realpath(current_file)), relative_src_path))

  if platform.system() == 'Linux':
    if os.system('make tests'):
      exit()
  else:
    print 'Platform not supported.'
    exit()

  tests = glob.glob('out/Debug/*_test')
  for test in tests:
    if os.system(test) != 0:
      exit()
