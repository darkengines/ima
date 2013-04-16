#ifndef _ESTIMATORKERNEL_H_
#define _ESTIMATORKERNEL_H_

#include "Precision.h"
#include "TVector3.h"

class EstimatorKernel {
private:
public:
	virtual Real Compute(Real*, int) const = 0;
};

#endif