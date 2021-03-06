/*=============================================================================
Copyright (c) 2008 Broadcom Europe Limited.
All rights reserved.

Project  :  khronos
Module   :

FILE DESCRIPTION
Standalone GLSL compiler
=============================================================================*/
#ifndef GLSL_CONST_TYPES_H
#define GLSL_CONST_TYPES_H

#include "middleware/khronos/glsl/2708/glsl_fpu_4.h"

// Typedefs for built in types that can be compile time constants.

typedef uint8_t const_bool;
typedef uint8_t const_bvec2[2];
typedef uint8_t const_bvec3[3];
typedef uint8_t const_bvec4[4];
typedef int32_t const_int; // integers are signed and must fit into 17 bits
typedef int32_t const_ivec2[2];
typedef int32_t const_ivec3[3];
typedef int32_t const_ivec4[4];
typedef uint32_t const_float; // floats will be 32 bits on hardware
typedef uint32_t const_vec2[2];
typedef uint32_t const_vec3[3];
typedef uint32_t const_vec4[4];
typedef uint32_t const_mat2[2][2];
typedef uint32_t const_mat3[3][3];
typedef uint32_t const_mat4[4][4];

#define CONST_BOOL_TRUE 1
#define CONST_BOOL_FALSE 0

#define CONST_INT_SIGN_BIT 0x80000000

#define CONST_FLOAT_MINUS_ONE 0xbf800000
#define CONST_FLOAT_TWO 0x40000000
#define CONST_FLOAT_ONE 0x3f800000
#define CONST_FLOAT_ZERO 0x00000000

static INLINE const_bool const_bool_from_bool(const_bool b)
{
   return b;
}

static INLINE const_int const_int_from_bool(const_bool b)
{
   return b ? 1 : 0;
}

static INLINE const_float const_float_from_bool(const_bool b)
{
   return b ? CONST_FLOAT_ONE : CONST_FLOAT_ZERO;
}

static INLINE const_bool const_bool_from_int(const_int i)
{
   return i ? CONST_BOOL_TRUE : CONST_BOOL_FALSE;
}

static INLINE const_int const_int_from_int(const_int i)
{
   return i;
}

static INLINE const_float const_float_from_int(const_int i)
{
   const_float f;
   glsl_fpu_inttofloat((unsigned *)&f, i, 0);
   return f;
}

static INLINE const_bool const_bool_from_float(const_float f)
{
   f &= 0x7fffffff; // make -0.0f into 0.0f.
   return f ? CONST_BOOL_TRUE : CONST_BOOL_FALSE;
}

static INLINE const_int const_int_from_float(const_float f)
{
   const_int i;
   // "When constructors are used to convert a float to an int,
   // the fractional part of the floating-point value is dropped."
   // -- I take this to mean truncation, aka round to zero.
   glsl_fpu_floattointz((unsigned *)&i, f, 0);
   return i;
}

static INLINE const_float const_float_from_float(const_float f)
{
   return f;
}

#endif // CONST_TYPES_H
