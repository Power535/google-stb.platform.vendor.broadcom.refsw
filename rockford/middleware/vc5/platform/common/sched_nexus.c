/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.
=============================================================================*/

#include "sched_nexus.h"
#include "platform_common.h"
#include "gmem.h"
#include "vcos.h"

#include <stdlib.h>
#include <memory.h>
#include <pthread.h>

#include "bchp.h"
#include "bkni.h"
#include "bkni_multi.h"
#include "berr_ids.h"

#include "nexus_graphicsv3d.h"
#include "nexus_base_os.h"

#define MAX_PROXIES     16
#define MAX_COMPLETIONS 40

/* Static checks */
#define CASSERT(EXPR, MSG) typedef int assertion_failed_##MSG[(EXPR) ? 1 : -1]

/* Static checks on defines */
#define CHECK_DEFINE(BCM_NAME, NEXUS_NAME) \
   CASSERT(BCM_NAME == NEXUS_NAME, BCM_NAME##_not_equal_##NEXUS_NAME)

CHECK_DEFINE(V3D_MAX_BIN_SUBJOBS,            NEXUS_GRAPHICSV3D_MAX_BIN_SUBJOBS);
CHECK_DEFINE(V3D_MAX_RENDER_SUBJOBS,         NEXUS_GRAPHICSV3D_MAX_RENDER_SUBJOBS);
CHECK_DEFINE(V3D_MAX_QPU_SUBJOBS,            NEXUS_GRAPHICSV3D_MAX_QPU_SUBJOBS);
CHECK_DEFINE(V3D_IDENT_REGISTERS,            NEXUS_GRAPHICSV3D_MAX_IDENTS);
CHECK_DEFINE(BCM_SCHED_MAX_DEPENDENCIES,     NEXUS_GRAPHICSV3D_MAX_DEPENDENCIES);
CHECK_DEFINE(BCM_SCHED_MAX_COMPLETIONS,      NEXUS_GRAPHICSV3D_MAX_COMPLETIONS);

CHECK_DEFINE(V3D_MAX_GROUP_NAME_LEN,         NEXUS_GRAPHICSV3D_MAX_GROUP_NAME_LEN);
CHECK_DEFINE(V3D_MAX_COUNTER_NAME_LEN,       NEXUS_GRAPHICSV3D_MAX_COUNTER_NAME_LEN);
CHECK_DEFINE(V3D_MAX_COUNTER_UNIT_NAME_LEN,  NEXUS_GRAPHICSV3D_MAX_COUNTER_UNIT_NAME_LEN);
CHECK_DEFINE(V3D_MAX_COUNTERS_PER_GROUP,     NEXUS_GRAPHICSV3D_MAX_COUNTERS_PER_GROUP);

CHECK_DEFINE(V3D_MAX_EVENT_STRING_LEN,       NEXUS_GRAPHICSV3D_MAX_EVENT_STRING_LEN);

CHECK_DEFINE(GMEM_SYNC_CORE_READ,            NEXUS_GRAPHICSV3D_SYNC_CORE_READ);
CHECK_DEFINE(GMEM_SYNC_CORE_WRITE,           NEXUS_GRAPHICSV3D_SYNC_CORE_WRITE);
CHECK_DEFINE(GMEM_SYNC_CORE,                 NEXUS_GRAPHICSV3D_SYNC_CORE);
CHECK_DEFINE(GMEM_SYNC_TMU_DATA_READ,        NEXUS_GRAPHICSV3D_SYNC_TMU_DATA_READ);
CHECK_DEFINE(GMEM_SYNC_TMU_DATA_WRITE,       NEXUS_GRAPHICSV3D_SYNC_TMU_DATA_WRITE);
CHECK_DEFINE(GMEM_SYNC_TMU_DATA,             NEXUS_GRAPHICSV3D_SYNC_TMU_DATA);
CHECK_DEFINE(GMEM_SYNC_TMU_CONFIG_READ,      NEXUS_GRAPHICSV3D_SYNC_TMU_CONFIG_READ);
CHECK_DEFINE(GMEM_SYNC_QPU_IU_READ,          NEXUS_GRAPHICSV3D_SYNC_QPU_IU_READ);
CHECK_DEFINE(GMEM_SYNC_VCD_READ,             NEXUS_GRAPHICSV3D_SYNC_VCD_READ);
CHECK_DEFINE(GMEM_SYNC_TFU_READ,             NEXUS_GRAPHICSV3D_SYNC_TFU_READ);
CHECK_DEFINE(GMEM_SYNC_TFU_WRITE,            NEXUS_GRAPHICSV3D_SYNC_TFU_WRITE);
CHECK_DEFINE(GMEM_SYNC_TFU,                  NEXUS_GRAPHICSV3D_SYNC_TFU);
CHECK_DEFINE(GMEM_SYNC_V3D,                  NEXUS_GRAPHICSV3D_SYNC_V3D);
CHECK_DEFINE(GMEM_SYNC_CPU_READ,             NEXUS_GRAPHICSV3D_SYNC_CPU_READ);
CHECK_DEFINE(GMEM_SYNC_CPU_WRITE,            NEXUS_GRAPHICSV3D_SYNC_CPU_WRITE);
CHECK_DEFINE(GMEM_SYNC_CPU,                  NEXUS_GRAPHICSV3D_SYNC_CPU);

/* Static checks on enums */
#define CHECK_ENUM(BCM_NAME, NEXUS_NAME) \
   CASSERT((int)BCM_NAME == (int)NEXUS_NAME, BCM_NAME##_not_equal_##NEXUS_NAME)

CHECK_ENUM(BCM_SCHED_JOB_TYPE_NULL,             NEXUS_Graphicsv3dJobType_eNull);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_V3D_BIN,          NEXUS_Graphicsv3dJobType_eBin);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_V3D_RENDER,       NEXUS_Graphicsv3dJobType_eRender);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_V3D_USER,         NEXUS_Graphicsv3dJobType_eUser);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_V3D_TFU,          NEXUS_Graphicsv3dJobType_eTFU);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_FENCE_WAIT,       NEXUS_Graphicsv3dJobType_eFenceWait);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_FENCE_SIGNAL,     NEXUS_Graphicsv3dJobType_eFenceSignal);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_TEST,             NEXUS_Graphicsv3dJobType_eTest);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_USERMODE,         NEXUS_Graphicsv3dJobType_eUsermode);
CHECK_ENUM(BCM_SCHED_JOB_TYPE_NUM_JOB_TYPES,    NEXUS_Graphicsv3dJobType_eNumJobTypes);

CHECK_ENUM(BEGL_CtrAcquire,   NEXUS_Graphicsv3dCtrAcquire);
CHECK_ENUM(BEGL_CtrRelease,   NEXUS_Graphicsv3dCtrRelease);
CHECK_ENUM(BEGL_CtrStart,     NEXUS_Graphicsv3dCtrStart);
CHECK_ENUM(BEGL_CtrStop,      NEXUS_Graphicsv3dCtrStop);

CHECK_ENUM(BEGL_EventAcquire, NEXUS_Graphicsv3dEventAcquire);
CHECK_ENUM(BEGL_EventRelease, NEXUS_Graphicsv3dEventRelease);
CHECK_ENUM(BEGL_EventStart,   NEXUS_Graphicsv3dEventStart);
CHECK_ENUM(BEGL_EventStop,    NEXUS_Graphicsv3dEventStop);

CHECK_ENUM(BCM_EVENT_BEGIN,   NEXUS_Graphicsv3dEventBegin);
CHECK_ENUM(BCM_EVENT_END,     NEXUS_Graphicsv3dEventEnd);
CHECK_ENUM(BCM_EVENT_ONESHOT, NEXUS_Graphicsv3dEventOneshot);

CHECK_ENUM(BCM_EVENT_INT32,   NEXUS_Graphicsv3dEventInt32);
CHECK_ENUM(BCM_EVENT_UINT32,  NEXUS_Graphicsv3dEventUInt32);
CHECK_ENUM(BCM_EVENT_INT64,   NEXUS_Graphicsv3dEventInt64);
CHECK_ENUM(BCM_EVENT_UINT64,  NEXUS_Graphicsv3dEventUInt64);

/* Static checks on structures */
#define CHECK_STRUCT(BCM_NAME, NEXUS_NAME) \
   CASSERT(sizeof(struct BCM_NAME) == sizeof(NEXUS_NAME), mismatched_sizes_##BCM_NAME##_and_##NEXUS_NAME)

CHECK_STRUCT(bcm_sched_dependencies,            NEXUS_Graphicsv3dSchedDependencies);
CHECK_STRUCT(bcm_sched_query_response,          NEXUS_Graphicsv3dQueryResponse);

CHECK_STRUCT(bcm_sched_counter_desc,            NEXUS_Graphicsv3dCounterDesc);
CHECK_STRUCT(bcm_sched_counter_group_desc,      NEXUS_Graphicsv3dCounterGroupDesc);
CHECK_STRUCT(bcm_sched_group_counter_selector,  NEXUS_Graphicsv3dCounterSelector);
CHECK_STRUCT(bcm_sched_counter,                 NEXUS_Graphicsv3dCounter);

CHECK_STRUCT(bcm_sched_event_desc,              NEXUS_Graphicsv3dEventDesc);
CHECK_STRUCT(bcm_sched_event_field_desc,        NEXUS_Graphicsv3dEventFieldDesc);
CHECK_STRUCT(bcm_sched_event_track_desc,        NEXUS_Graphicsv3dEventTrackDesc);

static void UsermodeHandler(void *context, int param);
static void FenceDoneHandler(void *context, int param);
static void CompletionHandler(void *context, int param);
static void RunCompletionHandler(void *context);

typedef struct
{
   NEXUS_Graphicsv3dHandle    session;
   BKNI_EventHandle           hRunCallback;
   NEXUS_ThreadHandle         hTaskHandle;
   volatile bool              runCallbackExit;

   void  (*pfnUpdateOldestNFID)(uint64_t);
   sem_t throttle;
} SchedContext;

static void DummyCompletion(void *data, uint64_t jobId, NEXUS_Graphicsv3dJobStatus status)
{
   UNUSED(data);
   UNUSED(jobId);
   UNUSED(status);
}

static void *SchedOpen(void *context)
{
   SchedContext   *ctx = (SchedContext *)context;

   NEXUS_Graphicsv3dCreateSettings  settings;

   NEXUS_Graphicsv3d_GetDefaultCreateSettings(&settings);

   settings.sUsermode.context  = ctx;
   settings.sUsermode.param    = 0;
   settings.sUsermode.callback = UsermodeHandler;

   settings.sFenceDone.context  = ctx;
   settings.sFenceDone.param    = 0;
   settings.sFenceDone.callback = FenceDoneHandler;

   settings.sCompletion.context  = ctx;
   settings.sCompletion.param    = 0;
   settings.sCompletion.callback = CompletionHandler;

   ctx->session = NEXUS_Graphicsv3d_Create(&settings);
   BKNI_CreateEvent(&ctx->hRunCallback);

   ctx->pfnUpdateOldestNFID = NULL;

   ctx->runCallbackExit = false;
   ctx->hTaskHandle = NEXUS_Thread_Create("comp_handler", RunCompletionHandler, (void *)ctx, NULL);

   sem_init(&ctx->throttle, 0, 20);

   return ctx->session;
}

static void SchedClose(void *context, void *session)
{
   SchedContext   *ctx = (SchedContext *)context;

   if (session != NULL)
   {
      BKNI_EventHandle hRunCallback;

      sem_destroy(&ctx->throttle);

      /* Close the completion handler thread */
      ctx->runCallbackExit = true;
      BKNI_SetEvent(ctx->hRunCallback);         /* Ensure the thread loop is triggered */
      NEXUS_Thread_Destroy(ctx->hTaskHandle);   /* This does a join internally */
      hRunCallback = ctx->hRunCallback;
      ctx->hRunCallback = NULL;                 /* In case the callback fires after the event is destroyed */
      BKNI_DestroyEvent(hRunCallback);

      NEXUS_Graphicsv3d_Destroy((NEXUS_Graphicsv3dHandle)session);
   }
}

static void CopyJobBase(NEXUS_Graphicsv3dJobBase *to, const struct bcm_sched_job *from)
{
   to->uiJobId = from->job_id;
   to->eType = (NEXUS_Graphicsv3dJobType)from->job_type;
   memcpy(&to->sCompletedDependencies, &from->completed_dependencies, sizeof(to->sCompletedDependencies));
   memcpy(&to->sFinalizedDependencies, &from->finalised_dependencies, sizeof(to->sFinalizedDependencies));
   to->pfnCompletion = (NEXUS_Graphicsv3dCompletionFn)from->completion_fn;
   to->pCompletionData = from->completion_data;
   to->uiSyncFlags = from->sync_list.flags;

   // If there have been any CPU writes since the last call into the kernel, then add this information
   // to the job. If the kernel re-orders the jobs, then it'll have to handle this. Any CPU writes after
   // job submission that need to be flushed (e.g. by proxy-jobs) would need to signal this to the kernel
   // as well. There is no proxy-job mechanism yet.
   if (from->sync_list.sync_cpu_write && __atomic_exchange_n(from->sync_list.sync_cpu_write, 0, __ATOMIC_ACQUIRE))
      to->uiSyncFlags |= NEXUS_GRAPHICSV3D_SYNC_CPU_WRITE;
}

static void CopyTFUJob(NEXUS_Graphicsv3dJobTFU *to, const struct bcm_tfu_job *from)
{
   to->sInput.uiTextureType      = from->input.texture_type;
   to->sInput.uiByteFormat       = from->input.byte_format;
   to->sInput.uiEndianness       = from->input.endianness;
   to->sInput.uiComponentOrder   = from->input.component_order;
   to->sInput.uiRasterStride     = from->input.raster_stride;
   to->sInput.uiChromaStride     = from->input.chroma_stride;
   to->sInput.uiAddress          = from->input.address;
   to->sInput.uiChromaAddress    = from->input.chroma_address;
   to->sInput.uiFlags            = from->input.flags;

   to->sOutput.uiMipmapCount     = from->output.mipmap_count;
   to->sOutput.uiVerticalPadding = from->output.vertical_padding;
   to->sOutput.uiWidth           = from->output.width;
   to->sOutput.uiHeight          = from->output.height;
   to->sOutput.uiEndianness      = from->output.endianness;
   to->sOutput.uiByteFormat      = from->output.byte_format;
   to->sOutput.uiAddress         = from->output.address;
   to->sOutput.uiFlags           = from->output.flags;

   to->sCustomCoefs.uiY          = from->custom_coefs.a_y;
   to->sCustomCoefs.uiRC         = from->custom_coefs.a_rc;
   to->sCustomCoefs.uiBC         = from->custom_coefs.a_bc;
   to->sCustomCoefs.uiGC         = from->custom_coefs.a_gc;
   to->sCustomCoefs.uiRR         = from->custom_coefs.a_rr;
   to->sCustomCoefs.uiGR         = from->custom_coefs.a_gr;
   to->sCustomCoefs.uiGB         = from->custom_coefs.a_gb;
   to->sCustomCoefs.uiBB         = from->custom_coefs.a_bb;
}

static unsigned QueueJobBatch(void *session, const struct bcm_sched_job *jobs, unsigned num_jobs)
{
   NEXUS_Error err = NEXUS_SUCCESS;

   unsigned batchSize = 1;
   switch (jobs->job_type)
   {
   case BCM_SCHED_JOB_TYPE_NULL:
   {
      NEXUS_Graphicsv3dJobNull   nexusNullJob;

      CopyJobBase(&nexusNullJob.sBase, jobs);

      err = NEXUS_Graphicsv3d_QueueNull(session, &nexusNullJob);
      break;
   }

   case BCM_SCHED_JOB_TYPE_V3D_BIN:
   {
      NEXUS_Graphicsv3dJobBin      nexusBin;

      CopyJobBase(&nexusBin.sBase, jobs);

      nexusBin.uiNumSubJobs = jobs->driver.bin.n;
      memcpy(&nexusBin.uiStart, &jobs->driver.bin.start, sizeof(nexusBin.uiStart));
      memcpy(&nexusBin.uiEnd,   &jobs->driver.bin.end,   sizeof(nexusBin.uiEnd));
      nexusBin.uiOffset = jobs->driver.bin.offset;
      nexusBin.uiFlags  = jobs->driver.bin.workaround_gfxh_1181 ? NEXUS_GRAPHICSV3D_GFXH_1181 : 0;

      err = NEXUS_Graphicsv3d_QueueBin(session, &nexusBin);
      break;
   }

   case BCM_SCHED_JOB_TYPE_V3D_RENDER:
   {
      NEXUS_Graphicsv3dJobRender   nexusRender;

      CopyJobBase(&nexusRender.sBase, jobs);

      nexusRender.uiNumSubJobs = jobs->driver.render.n;
      memcpy(&nexusRender.uiStart, &jobs->driver.render.start, sizeof(nexusRender.uiStart));
      memcpy(&nexusRender.uiEnd,   &jobs->driver.render.end,   sizeof(nexusRender.uiEnd));

      nexusRender.uiFlags = jobs->driver.render.workaround_gfxh_1181 ? NEXUS_GRAPHICSV3D_GFXH_1181 : 0;

      /* Render jobs are a bit special because they control the throttling semaphore. When their completion
       * handler is run, the semaphore is pushed. This means that ALL render jobs we submit MUST have
       * a non-null completion handler. The kernel scheduler can optimize away any NULL callbacks which
       * means they may not get called - this would result in the throttle semaphore getting out of sync.
       * To avoid this, we ensure that ALL render jobs have a completion handler. */
      if (nexusRender.sBase.pfnCompletion == NULL)
      {
         nexusRender.sBase.pfnCompletion = DummyCompletion;
         nexusRender.sBase.pCompletionData = NULL;
      }

      err = NEXUS_Graphicsv3d_QueueRender(session, &nexusRender);
      break;
   }

   case BCM_SCHED_JOB_TYPE_V3D_TFU:
   {
      unsigned const maxBatchSize = 16;
      NEXUS_Graphicsv3dJobTFU nexusTFU[maxBatchSize];

      // Count consecutive TFU jobs.
      const struct bcm_sched_job *end = jobs + 1;
      for (; end != jobs + num_jobs; ++end)
      {
         if (end->job_type != BCM_SCHED_JOB_TYPE_V3D_TFU)
            break;
      }

      // Clamp to max batch size.
      batchSize = end - jobs;
      if (batchSize > maxBatchSize)
         batchSize = maxBatchSize;

      // Copy over the jobs.
      for (unsigned i = 0; i != batchSize; ++i)
      {
         CopyJobBase(&nexusTFU[i].sBase, &jobs[i]);
         CopyTFUJob(&nexusTFU[i], &jobs[i].driver.tfu);
      }

      err = NEXUS_Graphicsv3d_QueueTFU(session, batchSize, nexusTFU);
      break;
   }

   case BCM_SCHED_JOB_TYPE_FENCE_WAIT:
   {
      NEXUS_Graphicsv3dJobFenceWait   nexusWait;

      CopyJobBase(&nexusWait.sBase, jobs);
      nexusWait.iFence = jobs->driver.fence_wait.fence;

      err = NEXUS_Graphicsv3d_QueueFenceWait(session, &nexusWait);
      break;
   }

   case BCM_SCHED_JOB_TYPE_FENCE_SIGNAL:
   {
      NEXUS_Graphicsv3dJobFenceSignal nexusSignal;

      CopyJobBase(&nexusSignal.sBase, jobs);

      err = NEXUS_Graphicsv3d_QueueFenceSignal(session, &nexusSignal, jobs->driver.fence_signal.fence);
      break;
   }

   case BCM_SCHED_JOB_TYPE_TEST:
   {
      NEXUS_Graphicsv3dJobTest   nexusTest;

      CopyJobBase(&nexusTest.sBase, jobs);
      nexusTest.uiDelay = jobs->driver.test.delay;

      err = NEXUS_Graphicsv3d_QueueTest(session, &nexusTest);
      break;
   }

   case BCM_SCHED_JOB_TYPE_USERMODE:
   {
      NEXUS_Graphicsv3dJobUsermode  nexusUsermode;

      CopyJobBase(&nexusUsermode.sBase, jobs);
      nexusUsermode.pData = jobs->driver.usermode.data;

      err = NEXUS_Graphicsv3d_QueueUsermode(session, &nexusUsermode);
      break;
   }

   default:
      unreachable();
   }

   return err == NEXUS_SUCCESS ? batchSize : 0;
}

static BEGL_SchedStatus QueueJobs(void *context, void *session, const struct bcm_sched_job *jobs, unsigned num_jobs)
{
   while (num_jobs > 0)
   {
      unsigned done = QueueJobBatch(session, jobs, num_jobs);
      if (!done)
         return BEGL_SchedFail;
      jobs += done;
      num_jobs -= done;
   }
   return BEGL_SchedSuccess;
}

static BEGL_SchedStatus QueueBinRender(void *context, void *session, const struct bcm_sched_job *bin, const struct bcm_sched_job *render)
{
   SchedContext              *ctx = (SchedContext *)context;
   NEXUS_Error                err = NEXUS_SUCCESS;
   NEXUS_Graphicsv3dJobBin    nexusBin;
   NEXUS_Graphicsv3dJobRender nexusRender;

   sem_wait(&ctx->throttle);

   /* Copy bin job */
   CopyJobBase(&nexusBin.sBase, bin);

   nexusBin.uiNumSubJobs = bin->driver.bin.n;
   memcpy(nexusBin.uiStart, bin->driver.bin.start, sizeof(nexusBin.uiStart));
   memcpy(nexusBin.uiEnd,   bin->driver.bin.end,   sizeof(nexusBin.uiEnd));
   nexusBin.uiOffset     = bin->driver.bin.offset;
   nexusBin.uiFlags      = bin->driver.bin.workaround_gfxh_1181 ? NEXUS_GRAPHICSV3D_GFXH_1181 : 0;

   nexusBin.uiMinInitialBinBlockSize = bin->driver.bin.minInitialBinBlockSize;

   /* Copy render job */
   CopyJobBase(&nexusRender.sBase, render);

   nexusRender.uiNumSubJobs = render->driver.render.n;
   memcpy(nexusRender.uiStart, render->driver.render.start, sizeof(nexusRender.uiStart));
   memcpy(nexusRender.uiEnd,   render->driver.render.end,   sizeof(nexusRender.uiEnd));
   nexusRender.uiFlags      = render->driver.render.workaround_gfxh_1181 ? NEXUS_GRAPHICSV3D_GFXH_1181 : 0;

   /* Render jobs are a bit special because they control the throttling semaphore. When their completion
    * handler is run, the semaphore is pushed. This means that ALL render jobs we submit MUST have
    * a non-null completion handler. The kernel scheduler can optimize away any NULL callbacks which
    * means they may not get called - this would result in the throttle semaphore getting out of sync.
    * To avoid this, we ensure that ALL render jobs have a completion handler. */
   if (nexusRender.sBase.pfnCompletion == NULL)
   {
      nexusRender.sBase.pfnCompletion = DummyCompletion;
      nexusRender.sBase.pCompletionData = NULL;
   }

   err = NEXUS_Graphicsv3d_QueueBinRender(session,
                                          &nexusBin,
                                          &nexusRender);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}

/* Query
 * Used by driver to poll for job completion.
 */
static BEGL_SchedStatus Query(
   void *context,
   void *session,
   const struct bcm_sched_dependencies *completed_deps,
   const struct bcm_sched_dependencies *finalised_deps,
   struct bcm_sched_query_response *response)
{
   NEXUS_Error err;

   BSTD_UNUSED(context);
   BDBG_ASSERT(response != NULL);

   err = NEXUS_Graphicsv3d_Query(session, (NEXUS_Graphicsv3dSchedDependencies *)completed_deps,
                                          (NEXUS_Graphicsv3dSchedDependencies *)finalised_deps,
                                          (NEXUS_Graphicsv3dQueryResponse *)response);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}


static void UsermodeHandler(void *context, int param)
{
   SchedContext                 *ctx   = (SchedContext *)context;
   uint64_t                      jobId = 0;
   NEXUS_Graphicsv3dUsermode     usermode;

   BSTD_UNUSED(param);

   while (NEXUS_Graphicsv3d_GetUsermode(ctx->session, jobId, &usermode))
   {
      jobId = usermode.uiJobId;
      usermode.pfnCallback(usermode.pData);
   }
}

static void FenceDoneHandler(void *context, int param)
{
   SchedContext                       *ctx = (SchedContext *)context;
   NEXUS_Graphicsv3dFenceEventHandle   hEvent;

   BSTD_UNUSED(param);

   NEXUS_Graphicsv3d_GetPendingFenceEvent(ctx->session, &hEvent);

   while (hEvent != NULL)
   {
      BKNI_SetEvent((BKNI_EventHandle)hEvent);

      NEXUS_Graphicsv3d_GetPendingFenceEvent(ctx->session, &hEvent);
   }
}

static void CompletionHandler(void *context, int param)
{
   SchedContext *ctx = (SchedContext *)context;

   if (ctx->hRunCallback)
      BKNI_SetEvent(ctx->hRunCallback);
}

static void RunCompletionHandler(void *context)
{
   SchedContext   *ctx = (SchedContext *)context;

   do
   {
      BKNI_WaitForEvent(ctx->hRunCallback, BKNI_INFINITE);

      if (!ctx->runCallbackExit)
      {
         NEXUS_Graphicsv3dCompletionInfo   info;
         uint32_t                          numCompletions;
         NEXUS_Graphicsv3dCompletion       completions[MAX_COMPLETIONS];
         uint64_t                          finalized[MAX_COMPLETIONS];

         NEXUS_Graphicsv3d_GetCompletions(ctx->session, 0, NULL, MAX_COMPLETIONS, &info, &numCompletions, completions);
         do
         {
            for (uint32_t i = 0; i < numCompletions; i++)
            {
               if (completions[i].eType == NEXUS_Graphicsv3dJobType_eRender)
                  sem_post(&ctx->throttle);

               if (completions[i].pfnCallback != NULL)
                  completions[i].pfnCallback(completions[i].pData, completions[i].uiJobId, completions[i].eStatus);

               finalized[i] = completions[i].uiJobId;
            }

            // Let the driver know about latest "oldest not finalized job id".
            // Note: we must do this even if there were no completed jobs in the list.
            // The Magnum code doesn't pass out jobs that don't have completion handlers,
            // but will still hit this handler to update the oldest non-finalized id.
            if (ctx->pfnUpdateOldestNFID != NULL)
               ctx->pfnUpdateOldestNFID(info.uiOldestNotFinalized);

            // We are about to poll for completed jobs again, so we can ignore any
            // completion callbacks that have occurred recently.
            BKNI_ResetEvent(ctx->hRunCallback);

            // Tell Nexus that we have finalized the list it gave us, and to tell us about any more.
            NEXUS_Graphicsv3d_GetCompletions(ctx->session, numCompletions,
                                             numCompletions > 0 ? finalized : NULL,
                                             MAX_COMPLETIONS, &info, &numCompletions, completions);
         }
         while (numCompletions > 0);
      }
   }
   while (!ctx->runCallbackExit);
}

static void WaitFence(void *context, int fence)
{
   BKNI_EventHandle     ev;
   SchedContext        *ctx = (SchedContext *)context;

   BKNI_CreateEvent(&ev);

   NEXUS_Graphicsv3d_RegisterFenceWait(ctx->session, fence, (NEXUS_Graphicsv3dFenceEventHandle)ev);

   BKNI_WaitForEvent(ev, BKNI_INFINITE);

   NEXUS_Graphicsv3d_UnregisterFenceWait(ctx->session, fence);

   BKNI_DestroyEvent(ev);
}

static BEGL_FenceStatus WaitFenceTimeout(void *context, int fence, uint32_t timeoutms)
{
   BKNI_EventHandle     ev;
   SchedContext        *ctx = (SchedContext *)context;
   BERR_Code            err;
   BEGL_FenceStatus     ret;

   BKNI_CreateEvent(&ev);

   NEXUS_Graphicsv3d_RegisterFenceWait(ctx->session, fence, (NEXUS_Graphicsv3dFenceEventHandle)ev);

   err = BKNI_WaitForEvent(ev, timeoutms);

   NEXUS_Graphicsv3d_UnregisterFenceWait(ctx->session, fence);

   BKNI_DestroyEvent(ev);

   ret = err == BERR_SUCCESS ? BEGL_FenceSignaled : BEGL_FenceTimeout;

   return ret;
}

static void MakeFence(void *context, int *fence)
{
   SchedContext        *ctx = (SchedContext *)context;

   NEXUS_Graphicsv3d_FenceMake(ctx->session, fence);
}

static void SignalFence(void *context, int fence)
{
   SchedContext *ctx = (SchedContext *)context;

   if (fence != -1)
   {
      NEXUS_Graphicsv3d_FenceSignal(ctx->session, fence);
   }
}

static void CloseFence(void *context, int fence)
{
   SchedContext *ctx = (SchedContext *)context;

   if (fence != -1)
   {
      NEXUS_Graphicsv3d_FenceClose(ctx->session, fence);
   }
}

static void GetInfo(void *context, void *session, struct v3d_idents *info)
{
   NEXUS_Graphicsv3dInfo   nInfo;
   uint32_t                n;

   BSTD_UNUSED(context);
   BSTD_UNUSED(session);

   NEXUS_Graphicsv3d_GetInfo(&nInfo);

   for (n = 0; n < NEXUS_GRAPHICSV3D_MAX_HUB_IDENTS; n++)
   {
      info->hubIdent[n] = nInfo.uiHubIdent[n];
   }

   for (n = 0; n < NEXUS_GRAPHICSV3D_MAX_CORES * NEXUS_GRAPHICSV3D_MAX_IDENTS; n++)
   {
      if (n < V3D_MAX_CORES * V3D_IDENT_REGISTERS)
         info->ident[n] = nInfo.uiIdent[n];
   }
}

static void RegisterUpdateOldestNFID(void *context, void *session, void (*update)(uint64_t))
{
   BSTD_UNUSED(session);

   SchedContext *ctx = (SchedContext *)context;

   ctx->pfnUpdateOldestNFID = update;
}

/*****************************************************************************/
// Performance counters
static void GetPerfNumCounterGroups(void *context, void *session, uint32_t *numGroups)
{
   SchedContext   *ctx = (SchedContext *)context;

   if (numGroups != NULL)
      NEXUS_Graphicsv3d_GetPerfNumCounterGroups(ctx->session, numGroups);
}

static BEGL_SchedStatus GetPerfCounterGroupInfo(void *context, void *session, uint32_t group,
                                                struct bcm_sched_counter_group_desc *desc)
{
   SchedContext                        *ctx = (SchedContext *)context;
   uint32_t                            numGroups;

   NEXUS_Graphicsv3d_GetPerfNumCounterGroups(ctx->session, &numGroups);
   if (group >= numGroups)
      return BEGL_SchedFail;

   /* Nexus API limits the amount of data passing through to 4k  */
   /* so group description needs to be retrieved one by one      */
   /* We rely on the fact that Nexus and our counter structures are identical */
   NEXUS_Graphicsv3d_GetPerfCounterGroupInfo(ctx->session, group,
                                             desc->name,
                                             &desc->max_active_counters,
                                             &desc->total_counters);

   for (uint32_t counter_index = 0; counter_index < desc->total_counters; counter_index++)
   {
      NEXUS_Graphicsv3d_GetPerfCounterDesc(ctx->session, group, counter_index, (NEXUS_Graphicsv3dCounterDesc *)&desc->counters[counter_index]);
   }

   return BEGL_SchedSuccess;
}

static BEGL_SchedStatus SetPerfCounting(void *context, void *session, BEGL_SchedCounterState state)
{
   SchedContext                  *ctx = (SchedContext *)context;
   NEXUS_Error                   err;

   err = NEXUS_Graphicsv3d_SetPerfCounting(ctx->session, (NEXUS_Graphicsv3dCounterState)state);

   if (err == NEXUS_SUCCESS)
      return BEGL_SchedSuccess;
   else
      return BEGL_SchedFail;
}

static BEGL_SchedStatus ChoosePerfCounters(void *context, void *session, const struct bcm_sched_group_counter_selector *selector)
{
   SchedContext   *ctx = (SchedContext *)context;
   uint32_t        numGroups;

   NEXUS_Graphicsv3d_GetPerfNumCounterGroups(ctx->session, &numGroups);
   if (selector->group_index >= numGroups)
      return BEGL_SchedFail;

   NEXUS_Graphicsv3d_ChoosePerfCounters(ctx->session, (NEXUS_Graphicsv3dCounterSelector *)selector);

   return BEGL_SchedSuccess;
}

static uint32_t GetPerfCounterData(void *context, void *session, struct bcm_sched_counter  *counters,
                                   uint32_t max_counters, uint32_t reset_counts)
{
   SchedContext   *ctx = (SchedContext *)context;
   uint32_t       cnt = 0;

   /* We rely on the fact that Nexus and our counter structures are identical */
   NEXUS_Graphicsv3d_GetPerfCounterData(ctx->session, max_counters, reset_counts, &cnt, (NEXUS_Graphicsv3dCounter*)counters);

   return cnt;
}

// Event timeline
static void GetEventCounts(void *context, void *session, uint32_t *numTracks, uint32_t *numEvents)
{
   SchedContext   *ctx = (SchedContext *)context;

   if (numTracks != NULL && numEvents != NULL)
      NEXUS_Graphicsv3d_GetEventCounts(ctx->session, numTracks, numEvents);
}

static BEGL_SchedStatus GetEventTrackInfo(void *context, void *session, uint32_t track,
                                          struct bcm_sched_event_track_desc *track_desc)
{
   SchedContext                     *ctx = (SchedContext *)context;
   NEXUS_Error                      err;

   BDBG_ASSERT(track_desc != NULL);

   err = NEXUS_Graphicsv3d_GetEventTrackInfo(ctx->session, track, (NEXUS_Graphicsv3dEventTrackDesc *)track_desc);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}

static BEGL_SchedStatus GetEventInfo(void *context, void *session, uint32_t event,
                                     struct bcm_sched_event_desc *event_desc)
{
   SchedContext  *ctx = (SchedContext *)context;
   NEXUS_Error    err;

   err = NEXUS_Graphicsv3d_GetEventInfo(ctx->session, event, (NEXUS_Graphicsv3dEventDesc *)event_desc);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}

static BEGL_SchedStatus GetEventDataFieldInfo(void *context, void *session, uint32_t event, uint32_t field,
                                              struct bcm_sched_event_field_desc *field_desc)
{
   SchedContext  *ctx = (SchedContext *)context;
   NEXUS_Error    err;

   err = NEXUS_Graphicsv3d_GetEventDataFieldInfo(ctx->session, event, field, (NEXUS_Graphicsv3dEventFieldDesc *)field_desc);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}

static BEGL_SchedStatus SetEventCollection(void *context, void *session, BEGL_SchedEventState state)
{
   SchedContext  *ctx = (SchedContext *)context;
   NEXUS_Error    err;

   err = NEXUS_Graphicsv3d_SetEventCollection(ctx->session, (NEXUS_Graphicsv3dEventState)state);

   return err == NEXUS_SUCCESS ? BEGL_SchedSuccess : BEGL_SchedFail;
}

static uint32_t GetEventData(void *context, void *session, uint32_t event_buffer_bytes, void *event_buffer,
                             uint32_t *lost_data, uint64_t *timestamp)
{
   SchedContext   *ctx = (SchedContext *)context;
   uint32_t       bytesCopied = 0;

   NEXUS_Graphicsv3d_GetEventData(ctx->session, event_buffer_bytes, event_buffer, lost_data, timestamp, &bytesCopied);

   return bytesCopied;
}

BEGL_SchedInterface *CreateSchedInterface(BEGL_MemoryInterface *memIface)
{
   SchedContext        *ctx   = NULL;
   BEGL_SchedInterface *iface = (BEGL_SchedInterface *)malloc(sizeof(BEGL_SchedInterface));

   if (iface == NULL)
      goto error;

   memset(iface, 0, sizeof(BEGL_SchedInterface));

   ctx = (SchedContext *)malloc(sizeof(SchedContext));

   if (ctx == NULL)
      goto error;

   memset(ctx, 0, sizeof(SchedContext));

   iface->context        = (void*)ctx;
   iface->memIface       = memIface;

   iface->Open             = SchedOpen;
   iface->Close            = SchedClose;
   iface->QueueJobs        = QueueJobs;
   iface->QueueBinRender   = QueueBinRender;
   /* iface->PollComplete     = PollComplete; */
   iface->Query            = Query;
   iface->WaitFence        = WaitFence;
   iface->WaitFenceTimeout = WaitFenceTimeout;
   iface->CloseFence       = CloseFence;

   iface->MakeFence        = MakeFence;
   iface->SignalFence      = SignalFence;

   iface->GetInfo          = GetInfo;

   iface->RegisterUpdateOldestNFID = RegisterUpdateOldestNFID;

   // Performance counters
   iface->GetPerfNumCounterGroups   = GetPerfNumCounterGroups;
   iface->GetPerfCounterGroupInfo   = GetPerfCounterGroupInfo;
   iface->SetPerfCounting           = SetPerfCounting;
   iface->ChoosePerfCounters        = ChoosePerfCounters;
   iface->GetPerfCounterData        = GetPerfCounterData;

   // Event timeline
   iface->GetEventCounts            = GetEventCounts;
   iface->GetEventTrackInfo         = GetEventTrackInfo;
   iface->GetEventInfo              = GetEventInfo;
   iface->GetEventDataFieldInfo     = GetEventDataFieldInfo;
   iface->SetEventCollection        = SetEventCollection;
   iface->GetEventData              = GetEventData;

   return iface;

error:
   if (iface != NULL)
      free(iface);

   if (ctx != NULL)
      free(ctx);

   return NULL;
}

void DestroySchedInterface(BEGL_SchedInterface *iface)
{
   if (iface != NULL)
   {
      if (iface->context != NULL)
      {
         memset(iface->context, 0, sizeof(SchedContext));
         free(iface->context);
      }

      memset(iface, 0, sizeof(BEGL_SchedInterface));

      free(iface);
   }
}
