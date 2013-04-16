#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <SDL.h>
#include <SDL_image.h>
#include "TVector3.h"
#include "THashTable.h"
#include "Btree.h"

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
	unsigned long* indices;
	Real* componants;
	Uint8 bytesPerPixel;
	void Save(const char*);
	void FillSpatialAndComponants(Real** buffer, unsigned long) const;
private:
};

#endif