import os
import sys
import os.path
import subprocess
import fileinput
import distutils

sys.path.insert(0, "buildtools/classes")

# These are SPM specific python objects
from Archive import *
from Binary import *
from DebBuilder import *
from Globals import *
from Installer import *
from Manual import *
from RLibrary import *
from System import *
from Version import *
from TestModel import *
from Version import *

"""
Print the usage for this build system
"""
def print_usage():  
  os.system( [ 'clear', 'cls' ][ os.name == 'nt' ] )
  print( '###########################################################')
  print( '# SPM Build System Usage                                  #')
  print( '###########################################################')
  print( 'Usage:')
  print( 'doBuild <build_target> <build_parameter>')
  print( '')
  print( 'Valid Build Types:')
  print( '  binary   - Build release executable')
  print( '  manual   - Build the user manual')
  print( '  clean    - Remove any previous debug/release build information')
  print( '  archive  - Build a zipped archive of the application')
  print( '  boost    - Build boost')
  print( '  check    - Do a check that prerequesite ppackages are installed')
  print( '  rlibrary - Build the R library')
  print( '  testmodel - Run the test suite of models')
  print( '  windows  - Build the Windows installer package')
  print( '  deb      - Create linux .deb installer')
  print( '')
  print( 'Other commands:')
  print( '  check    - Check build files')
  print( '  help     - Print help')
  return True


"""
Start our build system. This method essentially just
gets all of the system information we need and will
test that executables are in the path that we need
"""
def start_build_system():
  system_info.determine_compiler();

  if Globals.operating_system_ == "windows":
    Globals.cmd_path_      = system_info.find_exe_path('cmd.exe')
    Globals.python_cmd_    = "python"
    Globals.gfortran_path_ = system_info.find_exe_path('gfortran.exe')
    Globals.latex_path_    = system_info.find_exe_path('bibtex.exe')
    Globals.git_path_      = system_info.find_exe_path('git.exe')    
  else:
    Globals.python_cmd_    = "python3"
    Globals.gfortran_path_ = system_info.find_exe_path('gfortran')
    Globals.latex_path_    = system_info.find_exe_path('bibtex')
    Globals.git_path_      = system_info.find_exe_path('git')    
    if system_info.find_exe_path('unzip') == '':
      return Globals.PrintError('unzip is not in the current path. Please ensure it has been installed')
    if system_info.find_exe_path('cmake') == '':
      return Globals.PrintError('cmake is not in the current path. Please ensure it has been intalled')    
  system_info.set_new_path()

  if Globals.git_path_ == "":
    return Globals.PrintError("git is not in the current path. Please install a git command line client (e.g http://git-scm.com/downloads)")  
  if Globals.operating_system_ == 'windows' and os.path.exists(Globals.git_path_ + '\\sh.exe'):
  	return Globals.PrintError("git version has sh.exe in the same location. This will conflict with cmake. Please upgrade to a 64bit version of Git")    
  if not system_info.find_compiler_version():
    return False

  return True  

"""
Get the build information from the user
"""
def start():
  print('-- Checking for Timezone Python module')
  if'pytz' not in sys.modules:
    return Globals.PrintError("Python requires the module pytz for the build system to work")
  print('-- Checking for datetime Python module')
  if'datetime' not in sys.modules:
    return Globals.PrintError("Python requires the module datetime for the build system to work")
  print('-- Checking for re Python module')
  if 're' not in sys.modules:
    return Globals.PrintError("Python requires the module re for the build system to work")
  print('-- Checking for distutils Python module')
  if 'distutils' not in sys.modules:
    return Globals.PrintError("Python requires the module distutils for the build system to work")
  
  """
  Handle build information already passed in
  """
  if len(sys.argv) != 2:
      return Globals.PrintError('Please provide a single valid build target. Use doBuild help to see list');
  build_target = str(sys.argv[1])

  build_target = build_target.lower()    
  if build_target == "help":
    print_usage()
    return True
  if build_target == "check":
    print("--> All checks completed successfully")
    return True

  if not build_target.lower() in Globals.allowed_build_targets_:
    return Globals.PrintError(build_target + " is not a valid build target")
    
  Globals.build_target_ = build_target
  
  print( "-- Build target: " + Globals.build_target_)
  print( "")
  
  print( "-- Build target: Version")
  code_builder = Version()
  if not code_builder.start():
    return False      
  if Globals.build_target_ == "binary":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build")
    code_builder = Binary()
    if not code_builder.start():
      return False      
  elif Globals.build_target_ == "manual":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build")
    documentation_builder = Manual()
    if not documentation_builder.start():
      return False
  elif build_target == "clean":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Cleaning all SPM build files")
    clean = Clean()
    if not clean.clean():
      return False
  elif Globals.build_target_ == "archive":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build")
    archive_builder = Archive()
    if not archive_builder.start(build_parameters):
      return False
  elif Globals.build_target_ == "boost":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build")
    code_builder = Boost()
    if not code_builder.start():
      return False
  elif build_target == "rlibrary":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build"   )
    r_path = '' 
    if Globals.operating_system_ == 'windows':
      print( "find windows R")
      r_path = system_info.find_exe_path('R.exe') 
    else:
      r_path = system_info.find_exe_path('R')  
    print( "R path = " + r_path  )
    if r_path == '':
      return Globals.PrintError("R is not in the current path please add R to your path.")  
    rlibrary = Rlibrary()
    if not rlibrary.start():
      return False
  elif build_target == "testmodel":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Starting " + Globals.build_target_ + " Build")
    model_runner = ModelRunner()
    if not model_runner.start():
      return False
  elif build_target == "windows":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Building SPM Windows installer")
    if Globals.operating_system_ == 'linux':
      return Globals.PrintError('Building Windows installer under linux is not supported')
    installer = Installer()
    installer.start()
  elif build_target == "deb":
    print( "*************************************************************************")
    print( "*************************************************************************")
    print( "--> Building SPM .deb installer"    )
    if Globals.operating_system_ == 'windows':
      return Globals.PrintError('Building linux .deb under Windows is not supported')
    deb_builder = DebBuilder()
    if not deb_builder.start(build_parameters):
      return False    

  return True
  
"""
This is the entry point in to our build system
"""
system_info = SystemInfo()
if not start_build_system():
  system_info.reset_original_path()
  exit(1)
  
exit_code = 0
if not start():
  exit_code = 1
  
system_info.reset_original_path()
print("--> Finished ")
exit(exit_code)  
