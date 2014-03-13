#ifndef _GRAPHICS_DEVICE_H_
#define _GRAPHICS_DEVICE_H_

#include "SDL_ttf.h"		//used for text display
#include "Vectors.h"

#define MAKE_COLOR(R,G,B,A) ( (((B)&0xFF)<<0) | (((G)&0xFF)<<8) | (((R)&0xFF)<<16) | (((A)&0xFF)<<24) )

#define SUB_PIX(a) (ceil(a)-a)

const int SCREENWIDTH = 640;
const int SCREENHEIGHT = 480;

const int FPS = 60;

struct Pixel {
	Vector3 pos;
	int color;
};

class GraphicsDevice {
public:
	GraphicsDevice();

	void Init();
	void Draw();
	void Clear();

	void DrawTriangle(Vector3, Vector3, Vector3);

	void DrawLine(Vector3, Vector3, int);

	void DrawPixel(Pixel);

private:

	void Swap(Vector3 *, Vector3 *);

	void GraphicsDevice::DrawSegment(long, long);

	SDL_Surface* screen;
	Uint32* pixels;

	float Left_dXdY, Right_dXdY, LeftX, RightX;
};

#endif