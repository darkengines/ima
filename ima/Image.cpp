#include "Image.h"

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;

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
		Real factor = pow(2.0, bytesPerPixel);
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
	Real factor = pow(2.0, bytesPerPixel);
	while (i--) {
		pixels[i] = buffer[i]*factor;
	}
	SDL_UnlockSurface(surface);
	SDL_Surface *screen;
	SDL_SaveBMP(surface, name);
	SDL_FreeSurface(surface);
}