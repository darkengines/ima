#ifndef _GaussianMeanShiftKernel_H_
#define _GaussianMeanShiftKernel_H_

#include "MeanShiftKernel.h"
#include <stdlib.h>
#include <math.h>
#include "Core.h"
#include "THashTable.h"
#include "TVector.h"

class GaussianMeanShiftKernel: public MeanShiftKernel {
	
public:
	THashTable<TVector<Real>, Real>* cache;
	GaussianMeanShiftKernel();
	Real Compute(Real*,int, Real) const;
};

#endif