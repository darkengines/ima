#include "Image.h"

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;
typedef struct {
	Real x;
	Real y;
	Real r;
	Real g;
	Real b;
} xyrgb;

Image::Image() {
}
Image::Image(const char* src) {
	buffer = 0;
	SDL_Surface* surface = IMG_Load(src);
	if (!surface) {
		printf("Error loading image: %s\n", IMG_GetError());
	} else {
		bytesPerPixel = surface->format->BytesPerPixel;
		w = surface->w;
		h = surface->h;
		length =w * h;
		size = length * bytesPerPixel;
		buffer = (Real*)malloc(sizeof(Real)*size);
		Real factor = pow(2.0, 8)-1;
		if (bytesPerPixel == 3) {
			unsigned long i = length;
			rgb pixel;
			unsigned char temp;
			rgb* pixels = (rgb*)surface->pixels;
			unsigned long index = 0;
			while (i--) {
				index = 3 * i;
				memcpy(&pixel, pixels+i, 3);
				if (surface->format->Rshift < surface->format->Bshift) {
					temp = pixel.r;
					pixel.r = pixel.b;
					pixel.b = temp;
				}
				buffer[index] = pixel.r / factor;
				buffer[index+1] = pixel.g / factor;
				buffer[index+2] = pixel.b / factor;
			}
		}
		SDL_UnlockSurface(surface);
		SDL_FreeSurface(surface);
	}
}
Image::Image(const Image& src) {
	w = src.w;
	h = src.h;
	size = src.size;
	length = src.length;
	bytesPerPixel = src.bytesPerPixel;
	buffer = (Real*)malloc(sizeof(Real)*size);
	memcpy(buffer, src.buffer, sizeof(Real)*size);
}
Image::~Image() {
	if (buffer) delete buffer;
}
void Image::Save(const char* name) {
	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 24, 0, 0, 0, 0);
	unsigned long i = size;
	SDL_LockSurface(surface);
	unsigned char* pixels = (unsigned char*)surface->pixels;
	Real factor = pow(2.0, 8)-1;
	while (i--) {
		pixels[i] = buffer[i]*factor;
	}
	SDL_UnlockSurface(surface);
	SDL_SaveBMP(surface, name);
	SDL_FreeSurface(surface);
}
void Image::GetXyrgb(Real* dest, unsigned long index) {
	unsigned long n = index+1;
	dest[0] = (w - n % w)/(Real)w;
	dest[1] = (n / w)/(Real)h;
	index = index * bytesPerPixel;
	int i = bytesPerPixel;
	while (i--) {
		dest[i+2] = buffer[index+i];
	}
}
void Image::meanShiftProcess(Real* dest, unsigned long index, MeanShiftKernel& kernel, Real spatialTolerance, Real colorTolerance) {
	
	int size = 2+bytesPerPixel;
	Real* temp = 0;
	Real* vsum = 0;
	Real* xi = 0;
	Real* x = 0;
	Real sg, cg, g, ssum;
	ssum = 0;

	temp = (Real*)malloc(sizeof(Real)*size);
	vsum = (Real*)calloc(sizeof(Real),size);
	xi = (Real*)malloc(sizeof(Real)*size);
	x = (Real*)malloc(sizeof(Real)*size);

	GetXyrgb(x, index);
	int i = length;
	while (i--) {
		GetXyrgb(xi, i);
		sub(x, xi, temp, size);
		sg = kernel.Compute(temp, 2, spatialTolerance); //Optimize with cache
		cg = kernel.Compute(temp+2, bytesPerPixel, colorTolerance); //Optimize with cache
		g = -sg*cg;
		addScaled(vsum, xi, vsum, g, size);
		ssum += g;
	}
	mul(vsum, (1.0/ssum), dest, size);
	free(temp);
	free(vsum);
	free(xi);
	free(x);
}
void Image::MeanShift(Real* dest, unsigned long index, MeanShiftKernel& kernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses) {
	accuracy = accuracy * accuracy;
	int size = bytesPerPixel+2;
	Real* x = (Real*)malloc(sizeof(Real)*size);
	Real* y = (Real*)malloc(sizeof(Real)*size);
	Real* temp = (Real*)malloc(sizeof(Real)*size);
	Real m;
	unsigned long k = 0;
	GetXyrgb(x, index);
	do {
		memcpy(y, x, size*sizeof(Real));
		meanShiftProcess(x, index, kernel, spatialTolerance, colorTolerance);
		sub(y, x, temp, size);
		m = squaredMagnitude(temp, size);
		k++;
	} while (k < maxPasses &&  m > accuracy);
	memcpy(dest, x, sizeof(Real)*size);
	free(x);
	free(y);
	free(temp);
}
void Image::FixNoise(MeanShiftKernel& kernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses) {
	Image image(*this);
	Real* temp = (Real*)malloc(sizeof(Real)*bytesPerPixel);
	unsigned long i = length;
	SDL_Surface *screen;
 
	screen = SDL_SetVideoMode(w, h, 24, SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}
	SDL_Surface* surface;
	while (i--) {
		image.MeanShift(temp, i, kernel, spatialTolerance, colorTolerance, accuracy, maxPasses);
		setPixel(temp+2, i);
		surface = GetSurface();
		SDL_BlitSurface(surface, 0, screen, 0);
		SDL_Flip(screen);
		SDL_FreeSurface(surface);
		printf("%d/%d\n", length-i, length);
	}
}
void Image::setPixel(Real* pixel, unsigned long index) {
	index = index * bytesPerPixel;
	int i = bytesPerPixel;
	while (i--) {
		buffer[index+i] = pixel[i];
	}
}
SDL_Surface* Image::GetSurface() {
	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 24, 0, 0, 0, 0);
	unsigned long i = size;
	SDL_LockSurface(surface);
	unsigned char* pixels = (unsigned char*)surface->pixels;
	Real factor = pow(2.0, 8)-1;
	while (i--) {
		pixels[i] = buffer[i]*factor;
	}
	SDL_UnlockSurface(surface);
	return surface;
}