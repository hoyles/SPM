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

class DebBuilder:
  do_build_ = "doBuild"
  def start(self, skip_building = 'false'):
    print('-- Starting Deb Builder')
    print('-- Skip Building Archive? ' + skip_building)

    if skip_building != 'false':
      if Globals.operating_system_ == "windows":
        self.do_build_ += '.bat'
      else:
        self.do_build_ = './' + self.do_build_ + '.sh'
      print('--> Building SPM Archive')
      print('-- Re-Entering the build sytem to build a release binary')
      if os.system(self.do_build_ + ' archive') != EX_OK:
        return Globals.PrintError('Failed to build SPM archive')

    p = subprocess.Popen(['git', '--no-pager', 'log', '-n', '1', '--pretty=format:%H%n%h%n%ci' ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    lines = out.split('\n')          
    if len(lines) != 3:
      return Globals.PrintError('Format printed by GIT did not meet expectations. Expected 3 lines but got ' + str(len(lines)))
    print('-- SPM Revision: ' + lines[1])
    if not os.path.exists('bin/linux/deb'):
      os.mkdir('bin/linux/deb')
    print(lines[1])
    folder = 'bin/linux/deb/spm'   
    os.system('rm -rf ' + folder)
    os.makedirs(folder + '/usr/local/bin')
    os.makedirs(folder + '/usr/local/lib')
    os.system('cp spm/spm ' + folder + '/usr/local/bin')
    os.makedirs(folder + '/usr/local/share/doc/spm')
    os.makedirs(folder + '/usr/local/share/doc/SPM')
    os.makedirs(folder + '/usr/local/share/doc/README')
    os.makedirs(folder + '/usr/local/share/doc/R-library/')
    os.makedirs(folder + '/usr/local/share/doc/Examples')
    os.system('cp ../Build/SPM.pdf ' + folder + '/usr/local/share/doc/spm/')
    os.system('cp ../README ' + folder + '/usr/local/share/doc/README/')     
    os.system('cp -a ../Examples/. ' + folder + '/usr/local/share/doc/Examples/')       
    os.system('cp ../"R-libraries"/spm_1.1.tar.gz ' + folder + '/usr/local/share/doc/R-library/spm_1.1.tar.gz')
    os.makedirs(folder + '/DEBIAN')
    control_file = open(folder + '/DEBIAN/control', 'w')
    control_file.write('Package: spm\n')
    control_file.write('Version: 0x' + lines[1] + '\n')
    control_file.write('Section: base\n')
    control_file.write('Priority: optional\n')
    control_file.write('Architecture: amd64\n')
    control_file.write('Maintainer: Alistair Dunn <Alistair.Dunn@OceanEnvironmental.co.nz>\n')
    control_file.write('Description: SPM Spatial Population Model\n')
    control_file.close()

    if os.system('dpkg-deb --build ' + folder) != EX_OK:
      return Globals.PrintError('Failed to build DEB Package')
    return True
