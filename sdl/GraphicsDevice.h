#ifndef _GRAPHICS_DEVICE_H_
#define _GRAPHICS_DEVICE_H_

#include "SDL_ttf.h"		//used for text display
#include "Vectors.h"

#define MAKE_COLOR(R,G,B,A) ( (((B)&0xFF)<<0) | (((G)&0xFF)<<8) | (((R)&0xFF)<<16) | (((A)&0xFF)<<24) )

const int SCREENWIDTH = 640;
const int SCREENHEIGHT = 480;

const int FPS = 60;

struct Pixel {
	Vector2 pos;
	int color;
};

class GraphicsDevice {
public:
	GraphicsDevice();

	void Init();
	void Draw();
	void Clear();

	void DrawLine(Vector3, Vector3);

	void DrawPixel(Pixel);

private:
	SDL_Surface* screen;
	Uint32* pixels;
};

#endif