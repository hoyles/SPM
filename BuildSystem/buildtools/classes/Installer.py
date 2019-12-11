import os
import sys
import subprocess
import os.path
import fileinput
import re
from datetime import datetime, date
#from dateutil import tz

import Globals

EX_OK = getattr(os, "EX_OK", 0)

class Installer:
  def start(self):
    if Globals.operating_system_ == "windows":
      if os.path.exists(Globals.root_directory_ + '/Installer'):
        os.chdir(Globals.root_directory_ + '/Installer')
      print('--> Building SPM Archive')
      if os.system('"c:\\Progra~2\\NSIS\\makeNSIS.exe" SPM.nsi') != EX_OK:
        return Globals.PrintError('Failed to build the Installer')
    else:
      return Globals.PrintError('Installer is for generating the Windows installer only')
