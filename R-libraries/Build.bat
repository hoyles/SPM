R --vanilla < run-roxygen.R
R --vanilla < SPM_make_version.R

R CMD build --force spm
R CMD INSTALL --build spm

del spm.html

R --vanilla < spmTest_make_version.R
R CMD  build --force spmTest
R CMD  INSTALL --build spmTest

del spmTest.html



