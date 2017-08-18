/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#ifndef QPU_FLOAT_H
#define QPU_FLOAT_H

#include "libs/core/v3d/v3d_common.h"

EXTERN_C_BEGIN

uint32_t qpu_float_unpack16(uint32_t f16);
uint32_t qpu_float_pack16(uint32_t f32);

uint32_t qpu_fadd(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fsub(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fcmp(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fmul(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fmin(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fmax(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_fabs(uint32_t a); /* As done by abs unpack mode */
uint32_t qpu_fround(uint32_t a, bool prop_nan);
uint32_t qpu_ftrunc(uint32_t a, bool prop_nan);
uint32_t qpu_ffloor(uint32_t a, bool prop_nan);
uint32_t qpu_fceil(uint32_t a, bool prop_nan);
uint32_t qpu_ftoin(uint32_t a, bool *carry);
uint32_t qpu_ftoiz(uint32_t a, bool *carry);
uint32_t qpu_ftouz(uint32_t a, bool *carry);
uint32_t qpu_ftoc(uint32_t a, uint32_t quad_elnum);
uint32_t qpu_itof(uint32_t a);
uint32_t qpu_utof(uint32_t a);

uint32_t qpu_vfpack(uint32_t a, uint32_t b);
uint32_t qpu_vfmin(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_vfmax(uint32_t a, uint32_t b, bool prop_nan);
uint32_t qpu_vfmul(uint32_t a, uint32_t b, bool prop_nan);

EXTERN_C_END

#endif
