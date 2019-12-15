..\..\Build\windows\spm -p -q -g 0 -o profile.dat > profile.log
..\..\Build\windows\spm -s 10 -g 0 > simulation.log
..\..\Build\windows\spm -m -q -g 0 > MCMC.log
..\..\Build\windows\spm -e -q -g 0 -o MPD.dat > estimate.log
