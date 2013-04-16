#ifndef _ESTIMATOR_H_
#define _ESTIMATOR_H_

#include "Precision.h"
#include "TVector3.h"
#include "EstimatorKernel.h"
#include "Image.h"

class Estimator {
private:
	const Image& _image;
	const EstimatorKernel& _kernel;
	const Real& _tolerance;
	const Real& _tolerancePower;
public:
	Estimator(const Image&, const EstimatorKernel&, const Real&, const Real&);
	Estimator(const Estimator&);
	~Estimator();
	Real Compute(const TVector3<Real>&) const;
private:
};

#endif