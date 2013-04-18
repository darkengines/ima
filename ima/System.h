#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Image.h"

class System {
private:
public:
	System();
	System(const System&);
	~System();
	void Initialize();
	void Shutdown();
	void Run();
	Image& Debruit(Image&);
};

#endif