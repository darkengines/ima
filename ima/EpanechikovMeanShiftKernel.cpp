#include "EpanechikovMeanShiftKernel.h"

EpanechikovMeanShiftKernel::EpanechikovMeanShiftKernel(int mode, int cacheSize){

}

Real EpanechikovMeanShiftKernel::_compute(Real* componants, int length, Real tolerance) const {
	return squaredMagnitude(componants, length) < tolerance * tolerance;
}
Real EpanechikovMeanShiftKernel::getFrameSize(Real tolerance) {
	return tolerance;
}