cd lat_long_layer
..\..\..\Build\windows\spm -r -q -d  -g 0 > Expected.txt
cd ..
cd DijkstraDistance
..\..\..\Build\windows\spm -r -q -d  -g 0 > Expected.txt
cd ..
