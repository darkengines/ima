#ifndef _GaussianMeanShiftKernel_H_
#define _GaussianMeanShiftKernel_H_

#include "MeanShiftKernel.h"
#include <stdlib.h>
#include <math.h>
#include "Core.h"

class GaussianMeanShiftKernel: public MeanShiftKernel {
public:
	Real Compute(Real*,int, Real) const;
};

#endif