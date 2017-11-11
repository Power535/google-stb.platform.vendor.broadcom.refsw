/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
/*
   clean_boolean(int b)

   Implementation notes:

   'Cleans' a boolean value by converting any non zero value to 1.

   Preconditions:

   -

   Postconditions:

   result r is 0 or 1
*/

static INLINE bool clean_boolean(int b)
{
   return !!b;
}

/*
   clean_float(float f)

   Implementation notes:

   'Cleans' a floating point number by converting -INF to -FLT_MAX, INF to FLT_MAX and
   any NaN to zero. Khronos spec says nothing about what happens if an unclean float is
   passed to a function, so this at least guarantees reasonable behaviour.

   Preconditions:

   -

   Postconditions:

   result r is not NaN or infinite
*/

static INLINE float clean_float(float f)
{
   uint32_t u = float_to_bits(f);

   if (u == 0x7f800000)
      return FLT_MAX;
   if (u == (uint32_t)0xff800000)
      return -FLT_MAX;
   if ((u & 0x7f800000) == 0x7f800000)
      return 0;                              // force NaN to zero

   return f;
}

/*
   clampf(float x, float l, float u)

   Implementation notes:

   Khronos makes frequent use of the datatype clampf, which is defined as

      floating-point value clamped to [0, 1]

   This function is used in many places to clamp a floating point value to lie in a range
   (almost always 0..1) and also to implicitly remove any NaNs or infinities.

   Preconditions:

   l and u are not NaN or infinity and l <= u

   Postconditions:

   result r is not NaN or an infinity, and l <= r <= u
*/

static INLINE float clampf(float x, float l, float u)
{
   assert(l <= u);

   x = clean_float(x);

   return _maxf(_minf(x, u), l);
}

/*
   fixed_to_float(int f)

   Implementation notes:

   OpenGL ES 1.1 makes use of the datatype 'fixed' which is defined as a:

      signed 2s complement 16.16 scaled integer

   This function is used to convert a fixed value to its floating point equivalent, with
   some loss of precision. We believe this is justified as the state tables in the spec
   talk about storing all these values as type R (floating-point number).

   Preconditions:

   -

   Postconditions:

   result r is not NaN or an infinity
*/

static INLINE float fixed_to_float(int f)
{
   return (float)f / 65536.0f;
}

/*
   clampi(int x, int l, int u)

   Implementation notes:

   Preconditions:

   l <= u

   Postconditions:

   result r satisfies l <= r <= u
*/

static INLINE int clampi(int x, int l, int u)
{
   assert(l <= u);

   return (int) _max(_min( (int32_t)x, (int32_t)u), (int32_t)l);
}
