CC=g++

secuencial:
	$(CC) Manhatan_sec.cpp -o secuencial

opencl:
	$(CC) Manhatan_ocl.cpp LeerFuente.cpp utils_cl.cpp -o ocl -lOpenCL

all:
	make opencl 
	make secuencial

clean:
	rm -rf secuencial
	rm -rf ocl