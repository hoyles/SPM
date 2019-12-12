rem Basic numbers only model, no catch-at observations
casal -r -q > CASAL-run.log
..\..\..\Build\windows\spm -r -q > SPM-run.log
casal -e -q > CASAL-estimate.log
..\..\..\Build\windows\spm -e -q > SPM-estimate.log
