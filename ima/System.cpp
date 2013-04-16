#include "System.h"

System::System() {
}
System::System(const System& src) {
}
System::~System() {
}
void System::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Impossible d'initialiser SDL: %s\n", SDL_GetError());
		exit(1);
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) < 0) {
		fprintf(stderr, "Impossible d'initialiser IMG: %s\n", IMG_GetError());
		exit(1);
	}
}
void System::Shutdown() {
	IMG_Quit();
	SDL_Quit();
}
void System::Run() {
	EpanechikovEstimatorKernel ker(1);
	
	Image image("campus.jpg");
	Estimator est(image, ker, 1, 1);
	Real* test = 0;
	image.FillSpatialAndComponants(&test, 4000);
	Real* result = MeanShift(est,test,1,100000,image.bytesPerPixel+2);
	image.Save("caca.bmp");
}
Real* System::MeanShift(Estimator estimator, Real* x, Real precision, unsigned long maxIter, const int length) {
	int i, j;
	Real p = precision * precision;
	i = j = 0;
	Real n = 0;
	Real* y = (Real*)malloc(sizeof(Real)*length);
	
	do  {
		memcpy(y, x, length);
		estimator.Compute(y, x, length);
		while (j < length) {
			n += (y[j]-x[j])*(y[j]-x[j]);
			j++;
		}
		i++;
	} while (i >= maxIter || n >= p);
	memcpy(y, x, length);
	return y;
}