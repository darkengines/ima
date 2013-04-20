#include "EpanechikovMeanShiftKernel.h"

EpanechikovMeanShiftKernel::EpanechikovMeanShiftKernel(int mode, int cacheSize): MeanShiftKernel(mode, cacheSize) {

}

Real EpanechikovMeanShiftKernel::_compute(Real* componants, int length, Real tolerance) const {
	return squaredMagnitude(componants, length) < tolerance * tolerance;
}