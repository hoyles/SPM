import os
import sys
import subprocess
import os.path
import shutil
import fileinput
import re
from datetime import datetime, date

import Globals
from Binary import *

EX_OK = getattr(os, "EX_OK", 0)

"""
This class is responsible for building the main code base of the project
"""
class Archive:
  cmake_compiler_ = ""
  output_directory_ = "";
  do_build_ = "doBuild"
  def start(self, skip_building = 'false'):
    if Globals.operating_system_ == "windows":
      self.do_build_ += '.bat'
    else:
      self.do_build_ = './' + self.do_build_ + '.sh'
    binary_name = 'spm'
    extension = ''
    if Globals.operating_system_ == "windows":
      binary_name += '.exe'
    if skip_building != 'true':
      print('--> Building spm')
      print('-- Re-Entering the build system to build spm binary')
      print('-- All output is being diverted to binary_build.log')
      if os.system(self.do_build_ + ' binary > binary_build.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build the binary. Please check binary_build.log for error')

      print('--> Building manual')
      print('-- Re-Entering the build system to build the manual')
      print('-- All output is being diverted to manual_build.log')
      if os.system(self.do_build_ + ' manual > manual_build.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build the manual. Please check manual_build.log for error')
      os.system('rm -rf manual_build.log')

    print('--> Building R Libraries')
    print('-- Re-Entering the build system to build the R Libraries')
    print('-- All output is being diverted to Rlibraries_build.log')
    if os.system(self.do_build_ + ' Rlibrary > Rlibrary_build.log 2>&1') != EX_OK:
      return Globals.PrintError('Failed to build the R Libraries. Please check Rlibrary_build.log for error')
    os.system('rm -rf Rlibraries_build.log')

    ## Now we actually do the zipping of the binary
    output_directory = "SPM/" + Globals.operating_system_ + "/archive/"
    if not os.path.exists(output_directory):
      os.makedirs(output_directory);
    print("-- Target output directory: " + output_directory)

    if os.path.exists(output_directory + "spm.tar.gz"):
      print("-- Removing old Archive")
      os.system("rm -rf " + output_directory + "spm.tar.gz")

    # Shitty stuff to make building archive easier
    binary_path = self.output_directory_ = os.path.normpath(os.getcwd()) + "/bin/" + Globals.operating_system_

    os.system('rm -rf spm')
    os.system('rm -rf spm.tar')
    os.system('rm -rf spm.zip')
    os.system('mkdir spm')
    os.makedirs('spm/R-Libraries')
    os.system('cp ' + binary_path + '/binary/' + binary_name + ' spm/' + binary_name)
    os.system('cp ' + binary_path + '/library_release/' + library_name + ' spm/spm_release' + extension)
    os.system('cp ' + binary_path + '/library_test/' + library_name + ' spm/spm_test' + extension)
    os.system('cp ../Documentation/Manual/spm.pdf spm/"spm.pdf"')
    print("-- Manual copied")
    os.system('cp ../Documentation/Manual/spm.syn spm/spm.syn')    
    os.system('cp ../Documentation/Manual/spm.syn')        
    os.system('cp -r ../Examples spm/Examples')
    print("-- Examples copied")
    os.system('cp -r ../README.txt spm/README.txt')
    ## deal with R libraries
    if Globals.operating_system_ == "windows":
      os.system('cp ../R-libraries/spm_1.0.zip spm/R-Libraries/spm.zip')
    else: 
      os.system('cp ../R-libraries/spm_1.0.tar.gz spm/R-Libraries/spm.tar.gz')
      
    if Globals.operating_system_ == "windows":
      os.system("zip -r spm.zip spm/*")
    else:
      os.system('tar cvf spm.tar spm/')
      os.system('gzip spm.tar')
      os.system('mv spm.tar.gz ' + output_directory + 'spm.tar.gz')

    return True #Success!
