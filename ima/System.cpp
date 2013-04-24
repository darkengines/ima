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
	Image image("baboon.jpg");
	GaussianMeanShiftKernel gker(3, 256);
	GaussianMeanShiftKernel eker(2, 256);
	image.FixNoise(eker, gker, 0.1, 0.4, 0.1, 10);
	image.Save("caca.bmp");
	getchar();
}