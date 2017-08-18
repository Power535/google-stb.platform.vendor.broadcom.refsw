/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#ifndef V3D_PARALLEL_H
#define V3D_PARALLEL_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define V3D_PARALLEL_MAX_THREADS 8

/* Perform the work of a subjob */
typedef void (*v3d_parallel_exec_fn)(void *subjob);

/* Initialise this module */
extern bool v3d_parallel_init(uint32_t max_threads);

/* Terminate this module */
extern void v3d_parallel_term(void);

/* Execute parallel job */
extern void v3d_parallel_exec(v3d_parallel_exec_fn exec, unsigned int num_threads, void *data[]);

/* Query number of threads available */
unsigned int v3d_parallel_get_num_threads();

#ifdef __cplusplus
}
#endif

#endif
