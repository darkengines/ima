#ifndef _GaussianMeanShiftKernel_H_
#define _GaussianMeanShiftKernel_H_

#include "MeanShiftKernel.h"
#include <stdlib.h>
#include <math.h>
#include "Core.h"

class GaussianMeanShiftKernel: public MeanShiftKernel {
	
public:
	GaussianMeanShiftKernel(int mode, int cacheSize);
	Real _compute(Real*,int, Real) const;
};

#endif