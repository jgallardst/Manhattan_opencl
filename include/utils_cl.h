#ifndef UTILS_CL_H
#define UTILS_CL_H

#include <CL/cl.h>
#include "include/LeerFuente.h"

cl_int ObtenerTiempoEjecucionEntreEventos(cl_event event_ini,cl_event event_fin,cl_ulong&tiempo);
cl_int ObtenerTiempoEjecucionEvento(cl_event event,cl_ulong&tiempo);
cl_int EjecutarKernel(cl_command_queue command_queue,cl_kernel kernel,cl_uint work_dim,const size_t *global_work_offset,const size_t *global_work_size,size_t *local_work_size,cl_uint num_events,const cl_event *event_wait_list,cl_event&event);
cl_int RecibirBuffer(cl_command_queue command_queue,cl_mem buffer,cl_bool blocking_read,size_t offset,size_t cb,void *ptr,cl_uint num_events,const cl_event *event_wait_list,cl_event&event);
cl_int EnviarBuffer(cl_command_queue command_queue,cl_mem buffer,cl_bool blocking_write,size_t offset,size_t cb,const void *ptr,cl_uint num_events,const cl_event *event_wait_list,cl_event&event);
cl_int AsignarParametro(cl_kernel kernel,cl_uint arg_index,size_t arg_size,const void *arg_value);
cl_int CrearBuffer(cl_context context,cl_mem_flags flags,size_t size,void *host_ptr,cl_mem&buffer);
cl_int CrearKernel(cl_kernel&kernel,cl_program program,const char *kernel_name);
cl_int CrearPrograma(cl_program&program,cl_context context,cl_uint num_devices,const cl_device_id *device_list,const char *options,const char *fichero);
cl_int CrearCola(cl_context contexto,cl_device_id device_id,cl_command_queue_properties prop,cl_command_queue&cola);
cl_int CrearContexto(cl_platform_id platform,cl_device_id *devices,cl_uint num_devices,cl_context&contexto);
cl_int ObtenerDispositivos(cl_platform_id platform,cl_device_type device_type,cl_device_id *&devices,cl_uint&num_devices);
cl_int ObtenerPlataformas(cl_platform_id *&platforms,cl_uint&num_platforms);
void CodigoError(cl_int err);

#endif