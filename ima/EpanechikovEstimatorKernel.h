#ifndef _EpanechikovESTIMATORKERNEL_H_
#define _EpanechikovESTIMATORKERNEL_H_

#include "EstimatorKernel.h"

class EpanechikovEstimatorKernel: public EstimatorKernel {
private:
	const Real& tolerance;
public:
	EpanechikovEstimatorKernel(const Real& tolerance);
	EpanechikovEstimatorKernel(const EpanechikovEstimatorKernel&);
	~EpanechikovEstimatorKernel();
	Real Compute(const TVector3<Real>&) const;
};

#endif