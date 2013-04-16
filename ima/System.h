#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Image.h"
#include "EpanechikovEstimatorKernel.h"
#include "Estimator.h"

class System {
private:
public:
	System();
	System(const System&);
	~System();
	void Initialize();
	void Shutdown();
	void Run();
	Real* MeanShift(Estimator estimator, Real* x, Real precision, unsigned long maxIter, const int length);
};

#endif