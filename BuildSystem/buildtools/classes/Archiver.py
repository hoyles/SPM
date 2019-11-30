import os
import sys
import subprocess
import os.path
import shutil
import fileinput
import re
from datetime import datetime, date

import Globals
from Builder import *

EX_OK = getattr(os, "EX_OK", 0)

"""
This class is responsible for building the main code base of the project
"""
class Archiver:
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
      print('--> Building boost')
      print('-- Re-Entering build system to build boost')
      print('-- All output is being diverted to boost.log')
      print('-- Expected build time 10-60 minutes')
      if os.system(self.do_build_ + '  boost > boost.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build boost. Please check boost.log for the error')
      os.system('rm -rf boost.log')

      print('--> Building release version of SPM')
      print('-- Re-Entering the build system to build SPM')
      print('-- All output is being diverted to release_build.log')
      if os.system(self.do_build_ + ' release > release_build.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build spm. Please check release_build.log for the error')
      os.system('rm -rf release_build.log')

      print('--> Building documentation')
      print('-- Re-Entering the build system to build the documentation')
      print('-- All output is being diverted to documentation_build.log')
      if os.system(self.do_build_ + ' documentation > documentation_build.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build the documentation. Please check documenation_build.log for error')
      os.system('rm -rf documentation_build.log')

      print('--> Building test version of SPM')
      print('-- Re-Entering the build system to build a unit test library')
      print('-- All output is being diverted to unit_test_build.log')
      if os.system(self.do_build_ + ' library test > unit_test_build.log 2>&1') != EX_OK:
        return Globals.PrintError('Failed to build release library. Please check unit_test_build.log for the error')
      os.system('rm -rf unit_test_build.log')

    ## Now we actually do the zipping of the binary
    output_directory = "bin/" + Globals.operating_system_ + "/archive/"
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
    os.system('cp ' + binary_path + '/frontend/' + binary_name + ' spm/' + binary_name)
    os.system('cp ' + binary_path + '/library_release/' + library_name + ' spm/spm_release' + extension)
    os.system('cp ' + binary_path + '/library_adolc/' + library_name + ' spm/spm_adolc' + extension)
    os.system('cp ' + binary_path + '/library_betadiff/' + library_name + ' spm/spm_betadiff' + extension)
    os.system('cp ' + binary_path + '/library_cppad/' + library_name + ' spm/spm_cppad' + extension)
    os.system('cp ' + binary_path + '/library_test/' + library_name + ' spm/spm_test' + extension)
    os.system('cp ../Documentation/UserManual/spm.pdf spm/"spm User Manual.pdf"')
    print("-- Manual copied")
    os.system('cp ../Documentation/GettingStartedGuide/GettingStartedGuide.pdf spm/GettingStartedGuide.pdf')
    print("-- Starters' guide copied")
    os.system('cp ../Documentation/ContributorsManual/ContributorsGuide.pdf spm/ContributorsGuide.pdf')    
    print("-- Contributors' guide copied")
    os.system('cp ../Documentation/UserManual/spm.syn spm/spm.syn')    
    os.system('cp ../Documentation/UserManual/TextPad_syntax_highlighter.readme spm/TextPad_syntax_highlighter.readme')        
    os.system('cp -r ../Examples spm/Examples')
    print("-- Example code copied")
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
