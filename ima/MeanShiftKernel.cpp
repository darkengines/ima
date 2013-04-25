#include "MeanShiftKernel.h"

MeanShiftKernel::MeanShiftKernel(int mode, int cacheSize) {
	this->mode = mode;
	this->cacheSize = cacheSize;
	switch (mode) {
		case (KERNEL_2D): {
			cache2d = (cachedValue**)malloc(sizeof(cachedValue*)*cacheSize*2);
			int i, j;
			i = cacheSize*2;
			while (i--) {
				cache2d[i] = (cachedValue*)malloc(sizeof(cachedValue)*cacheSize*2);
				j=cacheSize*2;
				while (j--) {
					cache2d[i][j].computed = 0;
				}
			}
			break;
		}
		case (KERNEL_3D): {
			cache3d = (cachedValue***)malloc(sizeof(cachedValue**)*cacheSize*2);
			int i, j, k;
			i = cacheSize*2;
			while (i--) {
				cache3d[i] = (cachedValue**)malloc(sizeof(cachedValue*)*cacheSize*2);
				j=cacheSize*2;
				while (j--) {
					cache3d[i][j] = (cachedValue*)malloc(sizeof(cachedValue)*cacheSize*2);
					k = cacheSize*2;
					while(k--) {
						cache3d[i][j][k].computed = 0;
					}
				}
			}
			break;
		}
		default:
			break;
		}
}
Real MeanShiftKernel::Compute(Real* componants, int length, Real tolerance) {
	bool found = 0;
	cachedValue* c;
	int index;
	switch (mode) {
		case (KERNEL_2D): {
			int x = componants[0]*(cacheSize-1);
			if (x < 0) x = -x+cacheSize;
			index = componants[1]*(cacheSize-1);
			if (index < 0) index = -index+cacheSize;
			c = cache2d[x]+index;
			break;
		}
		case (KERNEL_3D): {
			int x = componants[0]*(cacheSize-1);
			if (x < 0) x = -x+cacheSize;
			int y = componants[1]*(cacheSize-1);
			if (y < 0) y = -y+cacheSize;
			index = componants[2]*(cacheSize-1);
			if (index < 0) index = -index+cacheSize;
			c = cache3d[x][y]+index;
			if (c->computed);
			break;
		}
		default:
			break;
	}
	if (c->computed) {
		return c->value;
	} else {
		c->value = _compute(componants, length, tolerance);
		c->computed = 1;
		return c->value;
	}
}