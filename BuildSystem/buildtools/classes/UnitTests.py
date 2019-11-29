import os
import sys
import subprocess
import os.path
import fileinput
import re
import time

import Globals

EX_OK = getattr(os, "EX_OK", 0)

class UnitTests:
  """
  Start the modelrunner builder
  """
  def start(self):
    binary_name = 'spm_unittests'
    if Globals.operating_system_ == 'windows':
      binary_name += '.exe'

    print('--> Looking for ' + Globals.build_directory_ + '/' + binary_name)
    if not os.path.exists(Globals.build_directory_ + '/' + binary_name):
      print('SPM binary was not found. Can not continue...')
      print('Please complete a release binary build before running the unit tests')
      return False

    start = time.time()

    result = False;
    print('--> Calling: ' + Globals.build_directory_ + "/spm_unittests --loglevel=all > spm_unittests.log 2>&1")
    print('')
    if os.system(Globals.build_directory_ + "/spm_unittests --loglevel=all > spm_unittests.log 2>&1") != EX_OK:
      elapsed = time.time() - start
      print('[FAILED] - ' + Globals.build_directory_ + ' in ' + str(round(elapsed, 2)) + ' seconds')
      result = True
    else:
      elapsed = time.time() - start
      print('[OK] - ' + Globals.build_directory_ + ' in ' + str(round(elapsed, 2)) + ' seconds')

    print('')
    if not result:
      print('Please check spm_unittests.log for more information')
      return False
    return True
