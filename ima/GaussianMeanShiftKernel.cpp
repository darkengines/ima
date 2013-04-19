#include "GaussianMeanShiftKernel.h"

GaussianMeanShiftKernel::GaussianMeanShiftKernel() {
}

Real GaussianMeanShiftKernel::Compute(Real* componants, int length, Real tolerance) const {
	TVector<unsigned char> vector(length);
	vector[0] = componants[0]*255;
	vector[0] = componants[1]*255;
	vector[0] = componants[2]*255;
		Real tolerance3 = 3*tolerance;
		if (squaredMagnitude(componants, length) >= (tolerance3*tolerance3)) return 0;
		Real* temp = (Real*)malloc(sizeof(Real)*length);
		mul(componants, 1/tolerance, temp, length);
		Real m = -(exp(-squaredMagnitude(temp, length)/2))/(2*tolerance);
		free(temp);
		return m;
}