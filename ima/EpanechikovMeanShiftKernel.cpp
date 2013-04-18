#include "EpanechikovMeanShiftKernel.h"

Real EpanechikovMeanShiftKernel::Compute(Real* componants, int length, Real tolerance) const {
	return squaredMagnitude(componants, length) < tolerance * tolerance;
}