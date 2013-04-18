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
		Real factor = pow(2.0, 8);
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
	Real factor = pow(2.0, 8);
	while (i--) {
		pixels[i] = buffer[i]*factor;
	}
	SDL_UnlockSurface(surface);
	SDL_SaveBMP(surface, name);
	SDL_FreeSurface(surface);
}
void Image::GetXyrgb(Real* dest, unsigned long index) {
	index = index * bytesPerPixel;
	dest[0] = index % w;
	dest[1] = index / w;
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
		GetXyrgb(xi, index);
		sub(x, xi, temp, size);
		sg = kernel.Compute(temp, 2, spatialTolerance);
		cg = kernel.Compute(temp+2, bytesPerPixel, colorTolerance);
		g = sg*cg;
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