#ifndef _CORE_H_
#define _CORE_H_

#include "Precision.h"

Real squaredMagnitude(Real* vector, int len);

void sub(Real* v1, Real* v2, Real* out, int len);

void add(Real* v1, Real* v2, Real* out, int len);

void addScaled(Real* v1, Real* v2, Real* out, Real s, int len);

void mul(Real* v, Real s, Real* out, int len);

#endif