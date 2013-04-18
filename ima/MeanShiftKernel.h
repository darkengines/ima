#ifndef _MeanShiftKernel_H_
#define _MeanShiftKernel_H_

#include "Precision.h"

class MeanShiftKernel {
private:
public:
	virtual Real Compute(Real*, int, Real) const = 0;
};

#endif