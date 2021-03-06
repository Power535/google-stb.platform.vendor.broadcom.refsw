/******************************************************************************
* SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
* Copyright (C) September 18, 2008 Silicon Graphics, Inc. All Rights Reserved.
* This document is licensed under the SGI Free Software B License Version
* 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
*******************************************************************************/

#ifndef __glplatform_h_
#define __glplatform_h_

/* $Revision: 10601 $ on $Date:: 2010-03-04 22:15:27 -0800 #$ */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This document is licensed under the SGI Free Software B License Version
 * 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
 */

/* Platform-specific types and definitions for OpenGL ES 1.X  gl.h
 *
 * Adopters may modify khrplatform.h and this file to suit their platform.
 * You are encouraged to submit all modifications to the Khronos group so that
 * they can be included in future versions of this file.  Please submit changes
 * by sending them to the public Khronos Bugzilla (http://khronos.org/bugzilla)
 * by filing a bug against product "OpenGL-ES" component "Registry".
 */

#include "../KHR/khrplatform.h"

#ifndef GL_API
#define GL_API      KHRONOS_APICALL
#endif

#ifndef GL_APIENTRY
#define GL_APIENTRY KHRONOS_APIENTRY
#endif

#ifdef __cplusplus
}
#endif

#endif /* __glplatform_h_ */
