import os
import os.path
import shutil
from distutils import dir_util

import Globals

class Builder():
  
  def start(self):     
    lapackFileName   = 'lapack-3.9.0'
    srcFileName      = Globals.root_directory_ + "/Source/MCMC/Children/kthohr/mcmc"

    # organise the libraries we want to copy over
    libraries = [ 'libblas.a',
                'liblapack.a' ]

    # Clean our any existing files if they already exist
    print('-- Cleaning Lapack files')
    if os.path.exists(lapackFileName):
        print("-- Deleting " + lapackFileName)
        shutil.rmtree(lapackFileName)
    if os.path.exists(srcFileName):
        print("-- Deleting: " + srcFileName)
        shutil.rmtree(srcFileName)    
    os.system('rm -rf ' + Globals.target_debug_lib_path_ + 'lib*')

    # Decompress our lapack archive
    if os.path.exists(lapackFileName + '.7z'):
        print('-- Decompressing Lapack: 7za.exe x lapack-3.9.0.7z 1> spm_lapack_unzip.log 2>&1')
        os.system('7za.exe x -bd -y lapack-3.9.0.7z 1> spm_lapack_unzip.log 2>&1') 
    else:
        return Globals.PrintError('Unable to Decompress lapack-3.9.0.7z')
    # Decompress our kthohr archive
    if os.path.exists(lapackFileName + '.7z'):
        print('-- Decompressing Lapack: 7za.exe mcmc.7z 1> spm_mcmc_unzip.log 2>&1')
        os.system('7za.exe x -bd -y mcmc.7z 1> spm_mcmc_unzip.log 2>&1') 
    else:
        return Globals.PrintError('Unable to Decompress mcmc.7z')
    
    dir_util.copy_tree('mcmc', srcFileName)

    # Build Boost
    print('-- Building Lapack - check casal2_build.log for progress')
    os.chdir(lapackFileName)
    os.mkdir('build')
    os.chdir('build')
    print('-- cmake command: ' + Globals.root_directory_ + '/BuildSystem/buildtools/windows/cmake/bin/cmake ' + Globals.cmake_compiler_ + ' .. ')
    os.system(Globals.root_directory_ + '/BuildSystem/buildtools/windows/cmake/bin/cmake ' + Globals.cmake_compiler_ + ' .. ')
    os.system('mingw32-make')
    
    for library in libraries:
      shutil.copy('lib/' + library, Globals.target_debug_lib_path_)

    return True
