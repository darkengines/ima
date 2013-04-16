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
	Image image("test.jpg");
	image.Save("caca.bmp");
	FlatEstimatorKernel kernel;
	Estimator estimator(image, kernel, 1, 1);
	TVector3<Real> v;
	v[0] = 0.5;
	v[1] = 0.5;
	v[2] = 0.5;
	Real result = estimator.Compute(v);
	printf("%f\n", result);
	getchar();
}