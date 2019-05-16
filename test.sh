#!/bin/bash

archivos=(pequeño.txt grande.txt)

for iter in {1..5}
do
    echo "Iteration $iter"
	for archivo in "${archivos[@]}"
	do
        echo "Secuencial $archivo"
        ./secuencial $archivo >> res/res_sec_$archivo_$iter.txt
        echo "ocl $archivo"
        ./ocl $archivo >> res/res_ocl_$archivo_$iter.txt
        echo "mpi $archivo"
		mpiexec -n 8 -f ./nodes ./mpi $archivo >> res/res_mpi_$archivo_$iter.txt
	done
done
