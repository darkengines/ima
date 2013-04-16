#include "EpanechikovEstimatorKernel.h"

EpanechikovEstimatorKernel::EpanechikovEstimatorKernel(const Real& tolerance): tolerance(tolerance) {
}
EpanechikovEstimatorKernel::EpanechikovEstimatorKernel(const EpanechikovEstimatorKernel& src): tolerance(src.tolerance) {
}
EpanechikovEstimatorKernel::~EpanechikovEstimatorKernel() {}
Real EpanechikovEstimatorKernel::Compute(Real* componants, int length) const {
	Real sum = 0;
	while (length--) {
		sum += componants[length]*componants[length];
	}
	return sum < tolerance * tolerance;
}