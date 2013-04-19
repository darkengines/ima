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
		bytesPerPixel = surface->format->BytesPerPixel+2;
		w = surface->w;
		h = surface->h;
		length =w * h;
		size = length * bytesPerPixel;
		buffer = (Real*)malloc(sizeof(Real)*size);
		Real factor = pow(2.0, 8)-1;
		if (bytesPerPixel == 5) {
			unsigned long i = length;
			rgb pixel;
			unsigned char temp;
			rgb* pixels = (rgb*)surface->pixels;
			unsigned long index = 0;
			while (i--) {
				index = 5 * i;
				memcpy(&pixel, pixels+i, 3);
				if (surface->format->Rshift < surface->format->Bshift) {
					temp = pixel.r;
					pixel.r = pixel.b;
					pixel.b = temp;
				}
				int j = i+1;
				buffer[index] = (w - j % w)/(Real)w;
				buffer[index+1] = (j / w)/(Real)h;
				buffer[index+2] = pixel.r / factor;
				buffer[index+3] = pixel.g / factor;
				buffer[index+4] = pixel.b / factor;
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
	unsigned long i = length;
	SDL_LockSurface(surface);
	unsigned char* pixels = (unsigned char*)surface->pixels;
	Real factor = pow(2.0, 8)-1;
	while (i--) {
		int surfaceIndex = i*(bytesPerPixel-2);
		int index = i*bytesPerPixel+2;
		pixels[surfaceIndex] = buffer[index]*factor;
		pixels[surfaceIndex+1] = buffer[index+1]*factor;
		pixels[surfaceIndex+2] = buffer[index+2]*factor;
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
void Image::meanShiftProcess(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance) {
	
	int size = bytesPerPixel;
	Real* temp = 0;
	Real* xi = 0;
	Real sg, cg, g, ssum;
	ssum = 0;

	temp = (Real*)malloc(sizeof(Real)*size);
	xi = (Real*)malloc(sizeof(Real)*size);

	memset(dest, 0, sizeof(Real)*size);

	int i = length;
	while (i--) {
		memcpy(xi, buffer+i*bytesPerPixel, bytesPerPixel*sizeof(Real));
		sub(src, xi, temp, size);
		sg = skernel.Compute(temp, 2, spatialTolerance); //Optimize with cache
		if (sg) {
			cg = ckernel.Compute(temp+2, bytesPerPixel-2, colorTolerance); //Optimize with cache
			g = -sg*cg;
			addScaled(dest, xi, dest, g, size);
			ssum += g;
		}		
	}
	mul(dest, (1.0/ssum), dest, size);
	free(temp);
	free(xi);
}
void Image::MeanShift(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses) {
	accuracy = accuracy * accuracy;
	int size = bytesPerPixel;
	Real* x = (Real*)malloc(sizeof(Real)*size);
	Real* temp = (Real*)malloc(sizeof(Real)*size);
	Real m;
	unsigned long k = 0;
	memcpy(x, src, size*sizeof(Real));
	do {
		memcpy(dest, x, size*sizeof(Real));
		meanShiftProcess(x, dest, skernel, ckernel, spatialTolerance, colorTolerance);
		sub(dest, x, temp, size);
		m = squaredMagnitude(temp, size);
		k++;
	} while (m > accuracy && (!maxPasses || k < maxPasses)  );
	memcpy(dest, x, sizeof(Real)*size);
	free(x);
	free(temp);
}
void Image::FixNoise(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses) {
	int size = bytesPerPixel;
	Image image(*this);
	Real* temp = (Real*)malloc(sizeof(Real)*bytesPerPixel);
	unsigned long i = length;
	SDL_Surface *screen;
 
	screen = SDL_SetVideoMode(w, h, 24, SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}
	unsigned char* screenS = (unsigned char*)screen->pixels;
	SDL_Surface* surface;
	while (i--) {
		int index = i*bytesPerPixel;
		screenS[i*3] = 0;
		screenS[i*3+1] = 255;
		screenS[i*3+2] = 0;
		
		SDL_Flip(screen);
		image.MeanShift(temp, image.buffer+index, skernel, ckernel, spatialTolerance, colorTolerance, accuracy, maxPasses);
		memcpy(buffer+index+2, temp+2, (bytesPerPixel-2)*sizeof(Real));
		surface = GetSurface();
		SDL_BlitSurface(surface, 0, screen, 0);
		SDL_Flip(screen);
		SDL_FreeSurface(surface);
		//printf("%d/%d\n", length-i, length);
	}
	free(temp);
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
	unsigned long i = length;
	SDL_LockSurface(surface);
	unsigned char* pixels = (unsigned char*)surface->pixels;
	Real factor = pow(2.0, 8)-1;
	while (i--) {
		int surfaceIndex = i*(bytesPerPixel-2);
		int index = i*bytesPerPixel+2;
		pixels[surfaceIndex] = buffer[index]*factor;
		pixels[surfaceIndex+1] = buffer[index+1]*factor;
		pixels[surfaceIndex+2] = buffer[index+2]*factor;
	}
	SDL_UnlockSurface(surface);

	return surface;
}