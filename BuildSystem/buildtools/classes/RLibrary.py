import os
import sys
import subprocess
import os.path
import fileinput
import re
import time

import Globals

EX_OK = getattr(os, "EX_OK", 0)

class Rlibrary:
  """
  Build the R library
  """
  def start(self):
    library_name = 'spm'
    binary_name = 'spm'
    if Globals.operating_system_ == 'windows':
      binary_name += '.exe'
   
    ## Check SPM is in your path. The R script needs it to get versions and that sort of information. 
    if not os.path.exists('SPM/' + Globals.operating_system_ + "/binary/" ):
      print('Looking for SPM/' + Globals.operating_system_ + "/binary/" + binary_name)
      print('\n\n*****************\nERROR\n*****************\nSPM binary was not found. Cannot continue\n\n')
      return False
    ## CHG Dir to R library
    os.chdir("../R-libraries/")  
    ## Run the oxygen
    os.system("R --vanilla < run-roxygen.R")   
    ## Build Namespace
    os.system("R --vanilla < SPM_make_version.R")
    ## build package
    os.system("R CMD build --force spm")
    os.system("R CMD INSTALL --build spm")
    os.system("R CMD check spm")
    os.system("del spm.html")
    return True
