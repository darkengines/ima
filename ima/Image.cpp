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
		Real xFactor = w - 1;
		Real yFactor = h - 1;
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
				buffer[index] = (i % w) / xFactor;
				buffer[index+1] = floor((Real)(i / h)) / yFactor;
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
void Image::meanShiftProcess(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real* tmpBuffer, int bufferSize) {
	
	Real* xi = 0;
	Real sg, cg, g, ssum;
	ssum = 0;

	

	memset(dest, 0, bufferSize);

	int i = length;
	while (i--) {
		sub(src, buffer+i*bytesPerPixel, tmpBuffer, bytesPerPixel);
		sg = skernel.Compute(tmpBuffer, 2, spatialTolerance); //Optimize with cache
		if (sg) {
			cg = ckernel.Compute(tmpBuffer+2, bytesPerPixel-2, colorTolerance); //Optimize with cache
			g = -sg*cg;
			addScaled(dest, buffer+i*bytesPerPixel, dest, g, bytesPerPixel);
			ssum += g;
		}		
	}
	mul(dest, (1.0/ssum), dest, bytesPerPixel);
}
void Image::MeanShift(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, Real* tmpBuffer) {
	accuracy = accuracy * accuracy;
	
	Real* x = tmpBuffer;
	Real* temp = tmpBuffer+bytesPerPixel;
	Real m;
	unsigned long k = 0;
	Real* tempBuffer = tmpBuffer+bytesPerPixel*2;
	memcpy(x, src, bytesPerPixel*sizeof(Real));
	do {
		memcpy(dest, x, bytesPerPixel*sizeof(Real));
		meanShiftProcess(x, dest, skernel, ckernel, spatialTolerance, colorTolerance, tempBuffer, sizeof(Real)*bytesPerPixel);
		sub(dest, x, temp, bytesPerPixel);
		m = squaredMagnitude(temp, bytesPerPixel);
		k++;
	} while (m > accuracy && (!maxPasses || k < maxPasses)  );
	memcpy(dest, x, sizeof(Real)*bytesPerPixel);
}
Image& Image::FixNoise(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, SDL_Surface* screen) {
	int size = bytesPerPixel;
	Image& image(*this);
	Real* temp = (Real*)malloc(sizeof(Real)*bytesPerPixel);
	Real* tmpBuffer = (Real*)malloc(sizeof(Real)*bytesPerPixel*3);
	unsigned long i = length;
	unsigned char* screenS = (unsigned char*)screen->pixels;
	SDL_Surface* surface;
	
	while (i--) {
		int index = i*bytesPerPixel;
		
		screenS[i*3] = 0;
		screenS[i*3+1] = 255;
		screenS[i*3+2] = 0;
		
		SDL_Flip(screen);

		MeanShift(temp, buffer+index, skernel, ckernel, spatialTolerance, colorTolerance, accuracy, maxPasses, tmpBuffer);
		memcpy(image.buffer+index, temp, (bytesPerPixel)*sizeof(Real));
		
		surface = image.GetSurface();
		SDL_BlitSurface(surface, 0, screen, 0);
		SDL_Flip(screen);
		SDL_FreeSurface(surface);
		//printf("%d/%d\n", length-i, length);
	}
	free(temp);
	free(tmpBuffer);
	return image;
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

Image& Image::Segment(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, SDL_Surface* screen) {
	Image noiseFix = *this;//FixNoise(skernel, ckernel, spatialTolerance, colorTolerance, accuracy, maxPasses, screen);
	Image& S(*this);
	GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
	Real squaredSpatialTolerance = spatialTolerance*spatialTolerance;
	Real squaredcolorTolerance = colorTolerance*colorTolerance;
	Real *temp = new Real[5];
	long index = noiseFix.length;
	long pixelIndex;
	long *sum = 0;
	unsigned char rgb[4];
	rgb[3] = '\0';
	unsigned char* cpy;
	long index2;
	long pixelIndex2;
	long *hp,*hq;

	hp = (long*)malloc(sizeof(long));
	hq = (long*)malloc(sizeof(long));

	//Compute histogram
	while (index--) {
		pixelIndex = index*bytesPerPixel+2;
		rgb[0] = floor(noiseFix.buffer[pixelIndex]*255.0);
		rgb[1] = floor(noiseFix.buffer[pixelIndex+1]*255.0);
		rgb[2] = floor(noiseFix.buffer[pixelIndex+2]*255.0);
		if (!g_hash_table_lookup_extended(hash, rgb, (gpointer*)&cpy, (gpointer*)&sum)) {
			sum = (long*)malloc(sizeof(long));
			*sum = 1;
		} else {
			(*sum)++;
		}
		g_hash_table_insert(hash, rgb, sum);
	}

	unsigned char* screenS = (unsigned char*)screen->pixels;
	SDL_Surface* surface;

	index =   length;
	while (index--) {
		screenS[index*3] = 0;
		screenS[index*3+1] = 255;
		screenS[index*3+2] = 0;
		
		SDL_Flip(screen);
		pixelIndex = index*bytesPerPixel;
		index2 = length;
		while (index2--) {
			pixelIndex2 = index2*bytesPerPixel;
			sub(noiseFix.buffer+pixelIndex, noiseFix.buffer+pixelIndex2, temp, bytesPerPixel);
			if (squaredMagnitude(temp, 2) < squaredSpatialTolerance && squaredMagnitude(temp+2, bytesPerPixel-2) < squaredcolorTolerance) {
				rgb[0] = floor(noiseFix.buffer[pixelIndex+2]*255.0);
				rgb[1] = floor(noiseFix.buffer[pixelIndex+3]*255.0);
				rgb[2] = floor(noiseFix.buffer[pixelIndex+4]*255.0);

				bool t = g_hash_table_lookup_extended(hash, rgb, (gpointer*)&cpy, (gpointer*)&hp);

				rgb[0] = floor(noiseFix.buffer[pixelIndex2+2]*255.0);
				rgb[1] = floor(noiseFix.buffer[pixelIndex2+3]*255.0);
				rgb[2] = floor(noiseFix.buffer[pixelIndex2+4]*255.0);

				g_hash_table_lookup_extended(hash, rgb, (gpointer*)&cpy, (gpointer*)&hq);

				memcpy(S.buffer+pixelIndex+2, *hp > *hq ? noiseFix.buffer+pixelIndex+2 : noiseFix.buffer+pixelIndex2+2, sizeof(Real)*(bytesPerPixel-2));
			}
		}
		surface = S.GetSurface();
		SDL_BlitSurface(surface, 0, screen, 0);
		SDL_Flip(screen);
		SDL_FreeSurface(surface);
	}
	free(temp);
	g_hash_table_destroy(hash);
	return S;
}