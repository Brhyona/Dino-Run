//Global.h to access globals across all files
#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glx.h>
#include "imageLoader.h"

class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	Image *platformImage;
	GLuint platformTexture;
	float xc[2];
	float yc[2];
};

class Global {
public:
	int xres, yres, paused;
	Texture tex;
	Global() {
		xres=640, yres=480, paused=0;
	}
};

extern Global g;

#endif //GLOBAL_H