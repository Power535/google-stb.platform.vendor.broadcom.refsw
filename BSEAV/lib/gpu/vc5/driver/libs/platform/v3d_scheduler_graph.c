/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include "v3d_scheduler_graph.h"
#include "v3d_scheduler.h"

#include <stdio.h>

static FILE *fp = NULL;

static void start_writing()
{
   // Seek back to overwrite the last '}'
   assert(fp != NULL);
   fseek(fp, -1, SEEK_CUR);
}

static void end_writing()
{
   fprintf(fp, "\n}");
   fflush(fp);
}

static const char *type_name(enum bcm_sched_job_type type)
{
   switch (type)
   {
   case BCM_SCHED_JOB_TYPE_NULL          : return "NULL";
   case BCM_SCHED_JOB_TYPE_V3D_BIN       : return "BIN";
   case BCM_SCHED_JOB_TYPE_V3D_RENDER    : return "RENDER";
   case BCM_SCHED_JOB_TYPE_V3D_USER      : return "USER";
   case BCM_SCHED_JOB_TYPE_V3D_TFU       : return "TFU";
#if !V3D_PLATFORM_SIM
   case BCM_SCHED_JOB_TYPE_FENCE_WAIT    : return "FENCE_WAIT";
#endif
   case BCM_SCHED_JOB_TYPE_TEST          : return "TEST";
   case BCM_SCHED_JOB_TYPE_USERMODE      : return "USERMODE";
   case BCM_SCHED_JOB_TYPE_V3D_BARRIER   : return "BARRIER";
   case BCM_SCHED_JOB_TYPE_WAIT_ON_EVENT : return "WAIT_EVENT";
   case BCM_SCHED_JOB_TYPE_SET_EVENT     : return "SET_EVENT";
   case BCM_SCHED_JOB_TYPE_RESET_EVENT   : return "RESET_EVENT";
#if V3D_USE_CSD
   case BCM_SCHED_JOB_TYPE_V3D_COMPUTE   : return "COMPUTE";
#endif
   default                               : return "error";
   }
}

static const char *type_color(enum bcm_sched_job_type type)
{
   switch (type)
   {
   case BCM_SCHED_JOB_TYPE_NULL        : return "gray88";
   case BCM_SCHED_JOB_TYPE_V3D_BIN     : return "aquamarine2";
   case BCM_SCHED_JOB_TYPE_V3D_RENDER  : return "crimson";
   case BCM_SCHED_JOB_TYPE_V3D_TFU     : return "goldenrod3";
#if !V3D_PLATFORM_SIM
   case BCM_SCHED_JOB_TYPE_FENCE_WAIT  : return "dodgerblue3";
#endif
   default                             : return "white";
   }
}

static void add_deps(const char *fromPrefix, uint64_t from, const struct bcm_sched_dependencies *deps, const char *color)
{
   for (unsigned int i = 0; i < deps->n; i++)
   {
      fprintf(fp, "%s%" PRIu64 " -> %" PRIu64 " [color=%s];\n",
                 fromPrefix, from, deps->dependency[i], color);
   }
}

void v3d_sched_graph_init(const char *filename)
{
   fp = fopen(filename, "w");
   if (fp == NULL)
   {
      fprintf(stderr, "Failed to open %s for writing\n", filename);
      return;
   }

   printf("Recording job nodes and dependencies to %s\n", filename);

   fprintf(fp, "digraph jobTree {\n");
}

void v3d_sched_graph_term()
{
   if (fp != NULL)
   {
      fclose(fp);
      fp = NULL;
   }
}

void v3d_sched_graph_add_node(const struct bcm_sched_job *job)
{
   start_writing();

   fprintf(fp, "%" PRIu64 " [label=\"%s (%" PRIu64 ")\" style=filled fillcolor=%s];\n",
      job->job_id, type_name(job->job_type), job->job_id, type_color(job->job_type));

   add_deps("", job->job_id, &job->completed_dependencies, "gray");
   add_deps("", job->job_id, &job->finalised_dependencies, "black");

#if !V3D_PLATFORM_SIM
   if (job->job_type == BCM_SCHED_JOB_TYPE_FENCE_WAIT)
   {
      fprintf(fp, "%" PRIu64 " -> F%d [color=darkorchid];\n",
         job->job_id, job->driver.fence_wait.fence);
   }
#endif

   end_writing();
}

#if !V3D_PLATFORM_SIM
void v3d_sched_graph_add_fence(
   int fence,
   const struct bcm_sched_dependencies *completed_deps,
   const struct bcm_sched_dependencies *finalised_deps)
{
   if (fence != V3D_PLATFORM_NULL_FENCE)
   {
      start_writing();

      fprintf(fp, "F%d [label=\"FENCE_SIGNAL (F%d)\" shape=box style=filled fillcolor=darkorchid];\n", fence, fence);

      add_deps("F", (uint64_t)fence, completed_deps, "gray");
      add_deps("F", (uint64_t)fence, finalised_deps, "black");

      end_writing();
   }
}
#endif

void v3d_sched_graph_add_bin_render_dep(uint64_t bin, uint64_t render)
{
   start_writing();

   fprintf(fp, "%" PRIu64 " -> %" PRIu64 " [color=darkgreen];\n", render, bin);

   end_writing();
}
