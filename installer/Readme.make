Readme.make for Spatial Population Model (SPM)
==============================================

The Spatial Population Model (SPM) is a generalised spatially explicit
age-structured population dynamics and movement model.

See the SPM User Manual for more information about the program.

License
=======

See the SPM User Manual for a copy of the software license.

To build SPM from the source files
==================================

To build SPM, issue the following commands in the SPM/BuildSystem/ directory

> doBuild help

to view available build options, then 

> doBuild boost
> doBuild release

to build the binary. 

Documentation, unittests, R Librarties, etc., can also be built with doBuild

Note that the compilation requires BOOST C++ libraries (version 1.71.0 is 
included in the archive). 

Currently, the locations of the version of BOOST and locations are 
hard-coded in the CMake CMakeLists.txt file. You will need to edit this file
if a different version is desired.

Other compilers may work, but we have not tested compilation using 
anything other than gcc 8.1.0 (x86_64-posix-seh-rev0).

Contact
=======

For more information about SPM, please read the SPM User Manual or contact
the authors.

