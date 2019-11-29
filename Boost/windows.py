import os
import os.path
import shutil
from distutils import dir_util

import Globals

class Builder():
  
  def start(self):    
    # Figure out GCC version
    pieces = Globals.compiler_version_.split('.')
    if len(pieces) != 3:
      return Globals.PrintError('Boost could not parse GCC Version properly: ' + Globals.compiler_version_)
    
    gcc_version = str(pieces[0]) + str(pieces[1])
    print('-- GCC Version for Boost: ' + gcc_version)
    
    boostFileName = Globals.boost_source_directory_ + '/' + Globals.boost_version
  
    # Decompress our boost archive
    if os.path.exists(boostFileName + '_windows.zip'):
        os.chdir(Globals.target_success_path_)
        print('-- Decompressing Boost: ' + boostFileName + '_windows.zip 1> spm_unzip.log 2>&1')
        #os.system('unzip ' + boostFileName + '_windows.zip 1> spm_unzip.log 2>&1') 
    else:
        return Globals.PrintError('Unable to Decompress ' + boostFileName + '_windows.zip')
    
    # Build Boost
    print('-- Building Boost - check spm_build.log for progress')
    folder = Globals.boost_directory_ + '/' + Globals.boost_version + '/'
    print('-- Folder: ' + folder)
    os.chdir(folder)
    os.system('bootstrap.bat gcc 1> spm_bootstrap.log 2>&1')
    os.system('b2.exe --toolset=gcc link=static runtime-link=static threading=multi -j ' + Globals.threads_ + ' 1> spm_build.log 2>&1')

    print('-- Completed Boost')
    
    return True
