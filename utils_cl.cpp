#include <stdio.h>
#include "include/utils_cl.h"

void CodigoError(cl_int err) {
	switch (err) {
		case CL_SUCCESS: printf("Ejecuci�n correcta\n"); break;
		case CL_BUILD_PROGRAM_FAILURE: printf("CL_BUILD_PROGRAM_FAILURE\n"); break;
		case CL_COMPILER_NOT_AVAILABLE: printf("CL_COMPILER_NOT_AVAILABLE\n"); break;
		case CL_DEVICE_NOT_AVAILABLE: printf("CL_DEVICE_NOT_AVAILABLE \n"); break;
		case CL_DEVICE_NOT_FOUND: printf("CL_DEVICE_NOT_FOUND\n"); break;
		case CL_INVALID_ARG_INDEX : printf("CL_INVALID_ARG_INDEX \n"); break;
		case CL_INVALID_ARG_SIZE : printf("CL_INVALID_ARG_SIZE \n"); break;
		case CL_INVALID_ARG_VALUE: printf("CL_INVALID_ARG_VALUE\n"); break;
		case CL_INVALID_BUFFER_SIZE : printf("CL_INVALID_BUFFER_SIZE \n"); break;
		case CL_INVALID_BUILD_OPTIONS: printf("CL_INVALID_BUILD_OPTIONS\n"); break;
		case CL_INVALID_COMMAND_QUEUE : printf("CL_INVALID_COMMAND_QUEUE \n"); break;
		case CL_INVALID_CONTEXT: printf("CL_INVALID_CONTEXT\n"); break;
		case CL_INVALID_DEVICE_TYPE: printf("CL_INVALID_DEVICE_TYPE\n"); break;
		case CL_INVALID_EVENT: printf("CL_INVALID_EVENT\n"); break;
		case CL_INVALID_EVENT_WAIT_LIST : printf("CL_INVALID_EVENT_WAIT_LIST \n"); break;
		case CL_INVALID_GLOBAL_WORK_SIZE : printf("CL_INVALID_GLOBAL_WORK_SIZE \n"); break;
		case CL_INVALID_HOST_PTR : printf("CL_INVALID_HOST_PTR \n"); break;
		case CL_INVALID_KERNEL: printf("CL_INVALID_KERNEL \n"); break;
		case CL_INVALID_KERNEL_ARGS : printf("CL_INVALID_KERNEL_ARGS \n"); break;
		case CL_INVALID_KERNEL_NAME: printf("CL_INVALID_KERNEL_NAME\n"); break;
		case CL_INVALID_MEM_OBJECT : printf("CL_INVALID_MEM_OBJECT \n"); break;
		case CL_INVALID_OPERATION : printf("\n"); break;
		case CL_INVALID_PLATFORM: printf("CL_INVALID_PLATFORM\n"); break;
		case CL_INVALID_PROGRAM : printf("CL_INVALID_PROGRAM \n"); break;
		case CL_INVALID_PROGRAM_EXECUTABLE : printf("CL_INVALID_PROGRAM_EXECUTABLE\n"); break;
		case CL_INVALID_QUEUE_PROPERTIES: printf("CL_INVALID_QUEUE_PROPERTIES\n"); break;
		case CL_INVALID_VALUE: printf("CL_INVALID_VALUE\n"); break;
		case CL_INVALID_WORK_DIMENSION : printf("CL_INVALID_WORK_DIMENSION \n"); break;
		case CL_INVALID_WORK_GROUP_SIZE : printf("CL_INVALID_WORK_GROUP_SIZE \n"); break;
		case CL_INVALID_WORK_ITEM_SIZE : printf("CL_INVALID_WORK_ITEM_SIZE \n"); break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE : printf("CL_MEM_OBJECT_ALLOCATION_FAILURE \n"); break;
		case CL_OUT_OF_HOST_MEMORY: printf("CL_OUT_OF_HOST_MEMORY\n"); break;
		case CL_OUT_OF_RESOURCES: printf("CL_OUT_OF_RESOURCES\n"); break;
		case CL_PROFILING_INFO_NOT_AVAILABLE : printf("CL_PROFILING_INFO_NOT_AVAILABLE \n"); break;
		default: printf("C�digo de error no contemplado\n"); break;
	}
}

cl_int ObtenerPlataformas(cl_platform_id *&platforms, cl_uint &num_platforms){
	cl_int error;
	int i;
	char cadena[100];

	error = clGetPlatformIDs(0, NULL, &num_platforms);

	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}

	platforms = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
	error = clGetPlatformIDs(num_platforms, platforms, NULL);

	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}

	for (int i = 0; i < num_platforms; i++){
		error = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 99, cadena, NULL);
		if(error != CL_SUCCESS) {
			CodigoError(error);
		return error;
		}
		printf("Nombre de la plataforma %d: %s \n", i + 1, cadena);
	}

	return CL_SUCCESS;



}

cl_int ObtenerDispositivos(cl_platform_id platform, cl_device_type device_type, cl_device_id *&devices, cl_uint &num_devices){
	cl_int error;
	int i;
	char cadena[100];

	error = clGetDeviceIDs(platform, device_type, 0, NULL, &num_devices);

	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}	

	devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
	error = clGetDeviceIDs(platform, device_type, num_devices, devices, NULL);

	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}	

	for(i = 0; i < num_devices; i++){
		error = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 99, cadena, NULL);
		if(error != CL_SUCCESS) {
			CodigoError(error);
			return error;
		}
		printf("%s\n", cadena);
	}
	return CL_SUCCESS;
}

cl_int CrearContexto(cl_platform_id platform, cl_device_id *devices, cl_uint num_devices, cl_context &contexto){
	cl_int error;
	cl_context_properties prop[3];
	prop[0] = CL_CONTEXT_PLATFORM;
	prop[1] = (cl_context_properties) platform;
	prop[2] = 0;
	contexto = clCreateContext(prop, num_devices, devices, NULL, NULL, &error);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}

cl_int CrearCola(cl_context contexto, cl_device_id device_id, cl_command_queue_properties prop, cl_command_queue &cola){
	cl_int error;
	cola = clCreateCommandQueue(contexto, device_id, prop, &error);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}
cl_int CrearPrograma(cl_program &program, cl_context context,cl_uint num_devices,
                                        const cl_device_id *device_list, const char *options, const char *fichero){
        char *codigo, errores[10000];
        cl_int error;
        leerFuentes(codigo, fichero);
        program=clCreateProgramWithSource(context, 1, (const char **)&codigo, NULL, &error);
        if(error != CL_SUCCESS){
                CodigoError(error);
                return error;
        }
        error=clBuildProgram(program, num_devices,device_list,options, NULL, NULL);
        if(error != CL_SUCCESS && error != CL_BUILD_PROGRAM_FAILURE){
                CodigoError(error);
                return error;
        }
        if(error = CL_BUILD_PROGRAM_FAILURE){
                error=clGetProgramBuildInfo(program, device_list[0],CL_PROGRAM_BUILD_LOG,10000,errores,NULL);
                if(error != CL_SUCCESS){
                        CodigoError(error);
                        return error;
                }
                printf("************************Errores de compilacion ************\n%s\n",errores);
                return CL_BUILD_PROGRAM_FAILURE;
        }
        return CL_SUCCESS;
}


cl_int CrearKernel(cl_kernel &kernel, cl_program program, const char *kernel_name){
	cl_int error;
	kernel = clCreateKernel(program, kernel_name, &error);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;					
}

cl_int CrearBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_mem &buffer){
	cl_int error;
	buffer=clCreateBuffer(context, flags, size, host_ptr, &error);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
	
}

cl_int AsignarParametro(cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value){
	cl_int error;
	error=clSetKernelArg(kernel, arg_index, arg_size, arg_value);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}

cl_int EnviarBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write,size_t offset, size_t cb, const void *ptr,
						 cl_uint num_events,const cl_event *event_wait_list, cl_event &event){
	cl_int error;
	error = clEnqueueWriteBuffer(command_queue, buffer, blocking_write, offset, cb, ptr, num_events, event_wait_list, &event);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}

cl_int RecibirBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read,size_t offset, size_t cb, void *ptr,
							cl_uint num_events,const cl_event *event_wait_list, cl_event &event){
	cl_int error;
	error = clEnqueueReadBuffer(command_queue, buffer, blocking_read, offset, cb, ptr, num_events, event_wait_list, &event);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}

cl_int EjecutarKernel (cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,const size_t *global_work_offset, 
						const size_t *global_work_size,size_t *local_work_size, cl_uint num_events,const cl_event *event_wait_list, cl_event &event){
	cl_int error;
	error = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events, event_wait_list, &event);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	return CL_SUCCESS;
}

cl_int ObtenerTiempoEjecucionEvento(cl_event event, cl_ulong &tiempo) {
	cl_int error;
	cl_ulong ini, fin;
	error = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &ini, NULL);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	error = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &fin, NULL);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	tiempo = fin - ini;
	return CL_SUCCESS;
}


cl_int ObtenerTiempoEjecucionEntreEventos(cl_event event_ini, cl_event event_fin, cl_ulong &tiempo){
	cl_int error;
	cl_ulong ini, fin;
	error = clGetEventProfilingInfo(event_ini, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &ini, NULL);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	error = clGetEventProfilingInfo(event_fin, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &fin, NULL);
	if(error != CL_SUCCESS) {
		CodigoError(error);
		return error;
	}
	tiempo = fin - ini;
	return CL_SUCCESS;
}

int main(int argc, char** argv){
	cl_platform_id *platforms;
	cl_uint num_platforms;
	cl_uint *num_devices;
	cl_device_id **devices;
	int i;
	const int N = 100000000;
	cl_float *v_in, *v_out;
	cl_context contexto;
	cl_command_queue cola;
	cl_program program;
	cl_kernel kernel;
	cl_mem buffer_in, buffer_out;
	cl_event e_ejec, e_envio, e_recep;

	v_in = (float*) malloc(N*sizeof(cl_float));
	v_out = (float*) malloc(N*sizeof(cl_float));

	for (i = 0; i < N; i++){
		v_in[i] = i;
	}

	ObtenerPlataformas(platforms, num_platforms);
	devices = (cl_device_id**)malloc(num_platforms*sizeof(cl_device_id*));
	num_devices = (cl_uint*)malloc(num_platforms * sizeof(cl_uint));
	for (i = 0; i < num_platforms; i++){
		ObtenerDispositivos(platforms[i], CL_DEVICE_TYPE_ALL, devices[i], num_devices[i]);
	}

	CrearContexto(platforms[0], devices[0], num_devices[0], contexto);
	CrearCola(contexto, devices[0][0], CL_QUEUE_PROFILING_ENABLE, cola);


	CrearPrograma(program, contexto, num_devices[0], devices[0], "", "kernel.cl");
	CrearKernel(kernel, program, "mult_vec2");

	CrearBuffer(contexto, CL_MEM_USE_HOST_PTR, (N*sizeof(cl_float)), v_in, buffer_in);
	CrearBuffer(contexto, CL_MEM_USE_HOST_PTR, (N*sizeof(cl_float)), v_out, buffer_out);
	AsignarParametro(kernel, 0, sizeof(cl_mem), &buffer_in);
	AsignarParametro(kernel, 1, sizeof(cl_mem), &buffer_out);

	size_t wi = N;
	//EnviarBuffer(cola, buffer_in, CL_FALSE, 0, N*sizeof(cl_float), v_in, 0, NULL, e_envio);
	EjecutarKernel(cola, kernel, 1, NULL, &wi, NULL, 0, NULL, e_ejec);
	//RecibirBuffer(cola, buffer_in, CL_FALSE, 0, N*sizeof(cl_float), v_out, 0, NULL, e_recep);
	clFinish(cola);

	cl_ulong tiempo;
	ObtenerTiempoEjecucionEntreEventos(e_ejec, e_recep, tiempo);
	printf("Tiempo total %ld ns \n", tiempo);


	free(platforms);
	free(num_devices);
	free(v_in);
	free(v_out);

	for (int i = 0; i < num_platforms; i++)
		free(devices[i]);
	free(devices);

	return 0;
}

