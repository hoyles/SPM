import os
import sys
import subprocess
import os.path
import fileinput
import re
from datetime import datetime, date
from dateutil import tz

import Globals

EX_OK = getattr(os, "EX_OK", 0)

class Installer:
  def start(self):
    if Globals.operating_system_ == "windows":

    else:
      self.do_build_ = './' + self.do_build_ + '.sh'

    print('--> Building SPM Archive')
    print('-- Re-Entering build system to build the archive')
    if os.system('"c:\Program files (x86)\NSIS\makeNSIS" SPM.nsi') != EX_OK:
      return Globals.PrintError('Failed to build the Installer')
