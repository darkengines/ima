#include "MeanShiftKernel.h"

MeanShiftKernel::MeanShiftKernel() {

}
Real MeanShiftKernel::Compute(Real* componants, int length, Real tolerance) {
	return _compute(componants, length, tolerance);
}