#ifndef _MeanShiftKernel_H_
#define _MeanShiftKernel_H_

#include "Precision.h"
#include <stdlib.h>

#define KERNEL_2D 2
#define KERNEL_3D 3

typedef struct {
	Real value;
	bool computed;
} cachedValue;

class MeanShiftKernel {
private:
	cachedValue*** cache3d;
	cachedValue** cache2d;
	int cacheSize;
	int mode;
public:
	MeanShiftKernel(int mode, int cacheSize);
	virtual Real Compute(Real*, int, Real);
	virtual Real _compute(Real*, int, Real) const = 0;
};

#endif