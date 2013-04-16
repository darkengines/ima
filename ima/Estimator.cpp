#include "Estimator.h"

Estimator::Estimator(const Image& image, const EstimatorKernel& kernel, const Real& tolerance, const Real& tolerancePower): _image(image), _kernel(kernel), _tolerance(tolerance), _tolerancePower(tolerancePower) {
}
Estimator::Estimator(const Estimator& src): _image(src._image), _kernel(src._kernel), _tolerance(src._tolerance), _tolerancePower(src._tolerancePower) {
}
Estimator::~Estimator() {
}
TVector3<Real>& Estimator::Compute(const TVector3<Real>& vector) const {
	TVector3<Real> sum1;
	Real g, sum2;
	g = sum2 = 0;
	sum1.SetAll(0);
	int i = 0;
	while (i < _image.length) {
		TVector3<Real> xi = _image.GetPixelAsVector3(i);
		g = _kernel.Compute(vector - xi);
		sum1 += xi*g;
		sum2 += g;
		i++;
	}
	return (sum1/sum2)-vector;
}