#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <SDL.h>
#include <SDL_image.h>
#include "THashTable.h"
#include "Btree.h"
#include "MeanShiftKernel.h"
#include "Core.h"
#include <math.h>

typedef SDL_PixelFormat PixelFormat;

class Image {
	
public:
	Image();
	Image(const char*);
	Image(const Image&);
	~Image();
	unsigned long w;
	unsigned long h;
	unsigned long length;
	unsigned long size;
	Real* buffer;
	int bytesPerPixel;
	void Save(const char*);
	void GetXyrgb(Real* dest, unsigned long index);
	void MeanShift(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses);
	void FixNoise(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses);
	void setPixel(Real* pixel, unsigned long index);
	SDL_Surface* GetSurface();
private:
	void meanShiftProcess(Real* dest,Real* src, MeanShiftKernel& gkernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance);
};

#endif