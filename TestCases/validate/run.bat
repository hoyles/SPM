cd CASAL-complex-1
..\..\..\Build\windows\spm -e -d > SPM-estimate.log
casal -e -q > CASAL-estimate.log
cd..
cd CASAL-dirichlet
..\..\..\Build\windows\spm -e -d > SPM-estimate.log
casal -e -q > CASAL-estimate.log
cd..
cd CASAL-complex-2
..\..\..\Build\windows\spm -r -d > SPM-run.log
casal -r -q > CASAL-run.log
..\..\..\Build\windows\spm -e -d > SPM-estimate.log
casal -e -q > CASAL-estimate.log
cd ..
cd CASAL-complex-3
..\..\..\Build\windows\spm -e -d > SPM-estimate.log
casal -e -q > CASAL-estimate.log
cd ..
cd SPM-movement-1
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-movement-2
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-movement-3
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-movement-4
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-movement-5
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-movement-6
..\..\..\Build\windows\spm -r -d > run.log
cd ..
cd SPM-simple
..\..\..\Build\windows\spm -e -d > Estimate.log
cd ..
cd SPM-simple-profile
..\..\..\Build\windows\spm -p -d > profile.log
cd ..
cd SPM-two-sex
..\..\..\Build\windows\spm -e -d > estimate.log
cd ..

