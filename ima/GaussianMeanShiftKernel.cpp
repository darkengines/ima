#include "GaussianMeanShiftKernel.h"

Real GaussianMeanShiftKernel::Compute(Real* componants, int length, Real tolerance) const {
	Real tolerance3 = 3*tolerance;
	if (squaredMagnitude(componants, length) >= (tolerance3*tolerance3)) return 0;
	Real* temp = (Real*)malloc(sizeof(Real)*length);
	mul(componants, 1/tolerance, temp, length);
	Real m = -(exp(-squaredMagnitude(temp, length)/2))/(2*tolerance);
	free(temp);
	return m;
}