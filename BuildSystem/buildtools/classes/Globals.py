"""
This module contains the list of global variables
"""
operating_system_ = ""
path_             = ""
compiler_path_    = ""
gfortran_path_    = ""
python_cmd_       = ""
compiler_ 		  = ""
compiler_version_ = ""
latex_path_       = ""
git_path_         = ""
cmd_path_         = ""
build_target_     = ""
threads           = "7"
SPM_version_number       = "1.1"
target_success_path_     = ""
target_include_path_     = ""
target_debug_lib_path_   = ""
target_release_lib_path_ = ""

allowed_build_targets_ = [ "binary", "manual", "clean", "archive", "boost", 
                           "check", "rlibrary", "runtest", "windows", "deb", 
                           "version"]

def PrintError(error_message):
  print("\n\n\n")
  print("###### ERROR ######")
  print( "Error Description:")
  print(error_message)
  print("###### ERROR ######")
  print("\n\n\n")
  return False
