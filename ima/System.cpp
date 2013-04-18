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
	Image image("lol.jpg");
	EpanechikovMeanShiftKernel ker;
	Real* result = (Real*)malloc(sizeof(Real)*5);
	image.MeanShift(result, 1, ker, 1, 1, 0.1, 5000);
	printf("x=%f y=%f r=%f g=%f b=%f\n", result[0], result[1], result[2], result[3], result[4]);
	image.Save("caca.bmp");
	getchar();
}