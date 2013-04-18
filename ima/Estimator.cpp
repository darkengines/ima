#include "Estimator.h"

Estimator::Estimator(const Image& image, const EstimatorKernel& kernel, const Real& tolerance, const Real& tolerancePower): _image(image), _kernel(kernel), _tolerance(tolerance), _tolerancePower(tolerancePower) {
}
Estimator::Estimator(const Estimator& src): _image(src._image), _kernel(src._kernel), _tolerance(src._tolerance), _tolerancePower(src._tolerancePower) {
}
Estimator::~Estimator() {
}
void Estimator::Compute(Real* input, Real* output, int length) const {
	Real* xi;
	Real* sum1 = (Real*)malloc(sizeof(Real)*length);
	Real* temp = (Real*)malloc(sizeof(Real)*length);
	Real g, g1, g2, sum2;
	g = g1 = g2 = sum2 = 0;

	int i, j;
	i=j=0;
	while (i<length) {
		sum1[i] = 0.0f;
		i++;
	}
	while (i < _image.length) {
		_image.FillSpatialAndComponants(&xi, i);
		
		j = 0;
		while (j<length) {
			temp[j] = input[j] - xi[j];
			j++;
		}

		g1 = _kernel.Compute(temp, 2);
		g2 = _kernel.Compute(temp+2, length-2);
		g = g1*g2;

		j = 0;
		while (j<length) {
			sum1[j] += xi[j] * g;
			j++;
		}

		sum2 += g;
		free(xi);
		i++;
	}
	j = 0;
	while (j<length) {
		output[j] = sum1[j] / sum2;
		j++;
	}
	free(temp);
	free(sum1);
}