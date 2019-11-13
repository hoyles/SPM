import os
import sys
import subprocess
import os.path
import shutil
import fileinput
import re
import time
import pytz
from datetime import datetime, date

import Globals

sys.path.insert(0, "../Boost")

EX_OK = getattr(os, "EX_OK", 0)

"""
This class is responsible for build BOOST
"""
class Boost:
  input_directory = ""
  output_directory_ = ""
  include_directory = ""
  
  def start(self):
    print("--> Starting build of Boost")
    print("-- Operating System: " + Globals.operating_system_)
    print("-- Compiler: " + Globals.compiler_)
  
    self.output_directory_ = os.path.normpath(os.getcwd()) + "/SPM/" + Globals.operating_system_ + "/Boost"
    print( "-- Checking if output folder structure exists: " + self.output_directory_)
    if not os.path.exists(self.output_directory_):
      print( "-- Creating output directory: " + self.output_directory_)
      os.makedirs(self.output_directory_)
    folder = self.output_directory_ + "/bin"
      
    self.include_directory = self.output_directory_ + "/include"
    if not os.path.exists(self.include_directory):
      print( "-- Creating include directory: " + self.include_directory)
      os.makedirs(self.include_directory)
      
    self.lib_directory = self.output_directory_ + "/lib"
    if not os.path.exists(self.lib_directory):
      print("-- Creating lib directory: " + self.lib_directory)
      os.makedirs(self.lib_directory)
      
    self.lib_debug_directory = self.lib_directory + "/debug"
    if not os.path.exists(self.lib_debug_directory):
      print( "-- Creating lib debug directory: " + self.lib_debug_directory)
      os.makedirs(self.lib_debug_directory)
      
    self.lib_release_directory = self.lib_directory + "/release"
    if not os.path.exists(self.lib_release_directory):
      print( "-- Creating lib release directory: " + self.lib_release_directory)
      os.makedirs(self.lib_release_directory)
   
    Globals.target_success_path_      = self.output_directory_ + '/'
    Globals.target_include_path_      = self.output_directory_ + '/include/'
    
    cwd = os.path.normpath(os.getcwd())    
    build_module_name = "build"    
    
    """
    # Handle loading the windows file and building this on windows
    """
    if Globals.operating_system_ == "windows":
      if not os.path.exists('../Boost/windows.py'):
        return Globals.PrintError('Boost library does not have a windows.py file.\nThis file is required to build this library on Windows')
      import windows as boost_builder
      builder = boost_builder.Builder()        
      print("")
      print( "--> Building Boost: " + folder)
      success = builder.start()
      if success:
        os.system('echo ' + str(builder.version_) + ' > ' + Globals.target_success_path_ + folder + '.success')
        print( "")
      else:
        return Globals.PrintError('Boost library ' + folder + ' had an error during the build. Check log files for more information')
      del sys.modules["windows"]
      
    else:
      if not os.path.exists('linux.py'):
        return Globals.PrintError('Boost library ' + folder + ' does not have a linux.py file.\nThis file is required to build this library on Linux')
      import linux as boost_builder
      builder = boost_builder.Builder()        
      print( "")
      print( "--> Building Boost: " + folder)
      success = builder.start()
      if success:
        os.system('echo ' + str(builder.version_) + ' > ' + Globals.target_success_path_ + folder + '.success')
        print( "")
      else:
        return Globals.PrintError('Boost library ' + folder + ' had an error during the build. Check log files for more information')
      del sys.modules["linux"]
          
    sys.path.pop()
    os.chdir(cwd)                
    if not success:
      return False
  
    if not found_build and Globals.build_parameters_ != "":
      return Globals.PrintError("Boost library " + Globals.build_parameters_ + " does not exist")
  
    print( "")
    print( "--> Boost libraries have been built successfully")
    return True


"""
This class is responsible building the main binaries
"""   
class Binary:
  def start(self):
    start = time.time()
    print( "--> Starting build of the front end code base")
    print( "--> Build configuration " + Globals.build_target_)
    print( "--> Operating System: " + Globals.operating_system_)
    
    self.cmake_compiler_ = [ 'Unix', 'MinGW' ][ Globals.operating_system_ == "windows" ]
    print( "--> CMake Compiler: " + self.cmake_compiler_)

    self.output_directory_ = "SPM/" + Globals.operating_system_ + "/" + Globals.build_target_ 
    
    if not os.path.exists(self.output_directory_):
      os.makedirs(self.output_directory_);
    print( "--> Target output directory: " + self.output_directory_)
    
    os.chdir(self.output_directory_);
    
    print( '--> Preparing CMake command')
    build_string = 'cmake -G "' + self.cmake_compiler_ + ' Makefiles" -S ../../../'
    print( "--> CMake command: " + build_string)
    if os.system(build_string) != EX_OK:
      return Globals.PrintError("Failed to execute cmake successfully to rebuild the make files")
    
    print( "--> Build main code base")
    if Globals.operating_system_ == "windows":
      if os.system("mingw32-make -j " + Globals.threads) != EX_OK:
        return Globals.PrintError("Failed to build code base. Please see above for build error")
    else:
      if os.system("make -j " + Globals.threads) != EX_OK:
        return Globals.PrintError("Failed to build code base. Please see above for build error")

    elapsed = time.time() - start
    print( 'Compile finished in ' + str(round(elapsed, 2)) + ' seconds')
    
    return True 

"""
This class is responsible for cleaning the build folders
"""   
class Clean:    
  def clean(self):
    print( '--> Starting clean of SPM build files')
    build_directory = os.path.normpath(os.getcwd()) + "/SPM/" + Globals.operating_system_ 
    if (os.path.exists(build_directory)):
      print( '--> Deleting folder: ' + build_directory)
      shutil.rmtree(build_directory)
    else:
      print( '--> Folder not found: ' + build_directory)
    return True
        
