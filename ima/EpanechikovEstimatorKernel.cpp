#include "EpanechikovEstimatorKernel.h"

EpanechikovEstimatorKernel::EpanechikovEstimatorKernel(const Real& tolerance): tolerance(tolerance) {
}
EpanechikovEstimatorKernel::EpanechikovEstimatorKernel(const EpanechikovEstimatorKernel& src): tolerance(src.tolerance) {
}
EpanechikovEstimatorKernel::~EpanechikovEstimatorKernel() {}
Real EpanechikovEstimatorKernel::Compute(const TVector3<Real>& vector) const {
	return vector.SquaredMagnitude() < tolerance * tolerance;
}