CC=g++
MPI=mpic++
all:
	make opencl 
	make secuencial
	make mpi

mpi:
	$(MPI) Manhatan_mpi_ocl.cpp LeerFuente.cpp utils_cl.cpp -o mpi -lOpenCL


secuencial:
	$(CC) Manhatan_sec.cpp -o secuencial

opencl:
	$(CC) Manhatan_ocl.cpp LeerFuente.cpp utils_cl.cpp -o ocl -lOpenCL


clean:
	rm -rf secuencial
	rm -rf ocl
	rm -rf mpi