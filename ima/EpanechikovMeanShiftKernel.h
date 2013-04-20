#ifndef _EpanechikovMeanShiftKernel_H_
#define _EpanechikovMeanShiftKernel_H_

#include "MeanShiftKernel.h"
#include "Core.h"
#include <stdlib.h>

class EpanechikovMeanShiftKernel: public MeanShiftKernel {
public:
	EpanechikovMeanShiftKernel(int mode, int cacheSize);
	Real _compute(Real*,int, Real) const;
};

#endif