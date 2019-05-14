CC=g++

all:
	make opencl 
	make secuencial

secuencial:
	$(CC) Manhatan_sec.cpp -o secuencial

opencl:
	$(CC) Manhatan_ocl.cpp LeerFuente.cpp utils_cl.cpp -o ocl -lOpenCL


clean:
	rm -rf secuencial
	rm -rf ocl