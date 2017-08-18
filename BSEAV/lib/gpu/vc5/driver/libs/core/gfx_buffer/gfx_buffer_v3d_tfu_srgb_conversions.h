/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#pragma once

#include "vcos.h"

EXTERN_C_BEGIN

/*
  tfu13 is nearly 13-bit unorm. It is used by the TFU for mipmap generation of
  sRGB textures.

  See v3d/hdl/v3d/tfu/src/v3d_tfusrgb_luts.c
*/

extern uint32_t gfx_buffer_srgb8_to_tfu13(uint32_t srgb);
extern uint32_t gfx_buffer_tfu13_to_srgb8(uint32_t tfu13);

EXTERN_C_END
