#include "Image.h"

//TODO: GENERALIZE
typedef struct {
	unsigned long index;
	unsigned char* data;
} btreeItem;

int cmpFunction(btreeItem a, btreeItem b) {
	return memcmp(a.data, b.data, 3);
}

void fillComponants(Real* componants, BTree<btreeItem>::node* node) {
	if (node) {
		Real r, g, b;
		r=g=b = 0.0;
		r = node->key_value.data[0] / 255.0;
		g = node->key_value.data[1] / 255.0;
		b = node->key_value.data[2] / 255.0;
		componants[node->key_value.index*3] = b;
		componants[node->key_value.index*3+1] = g;
		componants[node->key_value.index*3+2] = r;
		fillComponants(componants, node->left);
		fillComponants(componants, node->right);
	}
}

Image::Image() {
}
Image::Image(const char* src) {
	indices = 0;
	componants = 0;
	SDL_Surface* surface = IMG_Load(src);
	if (!surface) {
		printf("Error loading image: %s\n", IMG_GetError());
	} else {
		bytesPerPixel = surface->format->BytesPerPixel;
		w = surface->w;
		h = surface->h;
		length =w * h;
		size = length * bytesPerPixel;
		BTree<btreeItem>* btree = new BTree<btreeItem>();
		unsigned char* pixels = (unsigned char*) surface->pixels;
		btree->UserCmpFunction = cmpFunction;
		unsigned long i = 0;
		unsigned long j = 0;
		unsigned long index = 0;
		indices = (unsigned long*)malloc(length * sizeof(unsigned long));
		SDL_LockSurface(surface);
		btreeItem item;
		BTree<btreeItem>::node* n = 0;
		while (i<size) {
			item.data = pixels+i;
			n = btree->search(item);
			if (!n) {
				item.index = index;
				btree->insert(item);
				indices[j] = index;
				index++;
			} else {
				indices[j] = n->key_value.index;
			}
			i+= bytesPerPixel;
			j++;
		}
		componants = (Real*)malloc(sizeof(Real) * (index+1) * bytesPerPixel);
		fillComponants(componants, btree->Root());
		btree->destroy_tree();
		SDL_UnlockSurface(surface);
		
		SDL_FreeSurface(surface);
	}
}
Image::Image(const Image& src) {

}
Image::~Image() {
	if (indices) delete indices;
	if (componants) delete componants;
}
void Image::Save(const char* name) {
	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 24, 0, 0, 0, 0);
	unsigned long i = 0;
	unsigned long j = 0;
	SDL_LockSurface(surface);
	while (i<size) {
		((unsigned char*)(surface->pixels))[i] = *(componants+indices[j]*3)*255;
		((unsigned char*)(surface->pixels))[i+1] = *(componants+indices[j]*3+1)*255;
		((unsigned char*)(surface->pixels))[i+2] = *(componants+indices[j]*3+2)*255;
		i+= bytesPerPixel;
		j++;
	}
	SDL_UnlockSurface(surface);
	SDL_Surface *screen;
	SDL_SaveBMP(surface, name);
	SDL_FreeSurface(surface);
}

void Image::FillSpatialAndComponants(Real** buffer, unsigned long index) const {
	*buffer = (Real*)malloc(sizeof(Real)*(2+bytesPerPixel));
	Real x, y;
	x = index % w;
	y = index / w;
	*buffer[0] = x;
	*buffer[1] = y;
	int i = 0;
	while (i<bytesPerPixel) {
		*buffer[2+i] = componants[indices[index]*3+i];
		i++;
	}
}