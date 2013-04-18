#ifndef _EpanechikovMeanShiftKernel_H_
#define _EpanechikovMeanShiftKernel_H_

#include "MeanShiftKernel.h"
#include "Core.h"

class EpanechikovMeanShiftKernel: public MeanShiftKernel {
public:
	Real Compute(Real*,int, Real) const;
};

#endif