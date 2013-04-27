#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <SDL.h>
#include <SDL_image.h>
#include "THashTable.h"
#include "Btree.h"
#include "MeanShiftKernel.h"
#include "Core.h"
#include <math.h>
#include <glib.h>
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
	void MeanShift(Real* dest, Real* src, MeanShiftKernel& skernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, Real*, Real*, int);
	Image& FixNoise(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, SDL_Surface*);
	void setPixel(Real* pixel, unsigned long index);
	SDL_Surface* GetSurface();
	Image& Segment(MeanShiftKernel& skernel, MeanShiftKernel& ckernel,Real spatialTolerance, Real colorTolerance, Real accuracy, unsigned long maxPasses, SDL_Surface*);
private:
	void meanShiftProcess(Real* dest,Real* src, MeanShiftKernel& gkernel, MeanShiftKernel& ckernel, Real spatialTolerance, Real colorTolerance, Real*, int, Real*, int);
};

#endif