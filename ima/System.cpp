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
	Image image("lena_bruite_s0.1.jpg");
	GaussianMeanShiftKernel gker;
	GaussianMeanShiftKernel eker;
	//image.FixNoise(eker, gker, 0.015, 0.3, 0.001, 10);
	SDL_Surface *screen;
 
	screen = SDL_SetVideoMode(image.w, image.h, 24, SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}
	Image segmented = image.Segment(eker, gker, 16.0, 15.0, 0.001, 100000, screen);
	segmented.Save("caca.bmp");
	getchar();
}