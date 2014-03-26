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

	void ClearZBuffer();

private:

	void Swap(Vector3 *, Vector3 *);

	void GraphicsDevice::DrawSegment(long, long, int, float, float, float, float, float, float, float);

	SDL_Surface* screen;
	Uint32* pixels;
	Uint32* zBuffer;

	TTF_Font*	font;
	SDL_Color	txt_color;
	SDL_Surface* txt_img;
	SDL_Rect	txt_pos;
	std::string txt;

	Uint32 last;
};

#endif