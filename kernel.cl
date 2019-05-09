__kernel void countAppareances(int n, __global int* A, __global int* appereances)
{  
    __local int wg_app; 

  int id = get_global_id(0);
  int lid = get_local_id(0);
  int gid = get_group_id(0);
  if(lid == 0) wg_app = 0;
  
  if(A[id] == n){
    atomic_inc(&wg_app);
  }

  barrier(CLK_LOCAL_MEM_FENCE);

  if(lid == 0){
      appereances[gid] = wg_app;
  } 
}

__kernel void getSize(__global int* appereances, __global int* total){
  atomic_add(total, appereances[get_global_id(0)]);
}

__kernel void getData(int n, __global int* A,  __global int* appereances, __global int* data)
{  

  int id = get_global_id(0);
  int lid = get_local_id(0);
  int gid = get_group_id(0);
  int s = get_local_size(0);
  __local int index;

  if(gid == 0) {
    index = 0;
  } else {
    index = 0;
    for(int i = 0; i < gid; i++){
      index += appereances[i];
    }
  }

  if(lid == 0){
    for (int i = 0; i < s; i ++){
      if(A[id+i] == n){
          data[index] = id+i;
          index++;
      }
    }
  }
}

__kernel void mejores(int N, int total_size, __global int* data, __global int* mejor){
  int id = get_global_id(0);
  int lid = get_local_id(0);
  int gid = get_group_id(0);

  __local int max_wg;
  if(lid == 0){
    max_wg = 0;
  } 

  int max = 0;


  if(id < total_size){
    int source = data[id];

    for (int j = id+1; j < total_size; j++){
        int dest = data[j];
        int dist = abs(source / N - dest / N) + abs((source % N) - (dest % N));
        if (dist > max) max = dist;
    }
  }

  atomic_max(&max_wg, max);
  barrier(CLK_LOCAL_MEM_FENCE);


  if(lid == 0){
      mejor[gid] = max_wg;
  }


}