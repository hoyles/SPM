import os
import sys
import subprocess
import os.path
import fileinput
import re
import collections
import copy
from datetime import datetime, date
import pytz

import Globals

EX_OK = getattr(os, "EX_OK", 0)

class Manual:
  # Methods
  def __init__(self):
    print('--> Starting Documentation Builder')

  def start(self):
    # Figure out if we have Latex in our path, if we do then we want to
    # build the PDF documentation as well
    if Globals.latex_path_ != "":
      self.build_latex_ = True

    return Latex().Build()

class Documentation:
  # Methods
  def __init__(self):
    print('--> Starting Documentation Builder')

  def start(self):
    # Figure out if we have Latex in our path, if we do then we want to
    # build the PDF documentation as well
    if Globals.latex_path_ != "":
      self.build_latex_ = True

    return Latex().Build()

class Latex:
  def Build(self):

    print('-- Building latex documentation and pdf')
    os.chdir(Globals.root_directory_ + '/Documentation/Manual/')
    print('-- Building SPM.syn')
    os.system('python QuickReference.py')
    os.system('python SyntaxTextpad.py')
    os.system('python SyntaxNotepad++.py')

    for i in range(0,3):
      if Globals.operating_system_ != "windows":
        # Create SPM.aux
        if os.system('pdflatex --interaction=nonstopmode SPM') != EX_OK:
          return False
        # Create the bibliography
        if os.system('bibtex SPM') != EX_OK:
          return False
        # Fix the references/citations
        if os.system('pdflatex --halt-on-error --interaction=nonstopmode SPM') != EX_OK:
          return False
        if os.system('makeindex SPM') != EX_OK:
          return False
        if not os.path.exists('SPM.pdf'):
          return False
      else:
        if os.system('pdflatex.exe --enable-installer SPM') != EX_OK:
          return Globals.PrintError('pdflatex failed')
        if os.system('bibtex.exe SPM') != EX_OK:
          return Globals.PrintError('bibtex failed')
        if os.system('pdflatex.exe --halt-on-error --enable-installer SPM') != EX_OK:
          return Globals.PrintError('pdflatex failed')
        if os.system('makeindex.exe SPM') != EX_OK:
          return Globals.PrintError('makeindex failed')
    
    os.system("cp -f SPM.pdf " + Globals.root_directory_ + "/Build")
    os.system("cp -f SPM.syn " + Globals.root_directory_ + "/Build")
    os.system("cp -f SPM.xml " + Globals.root_directory_ + "/Build")
    
    print('-- Built the SPM manual')

    return True
