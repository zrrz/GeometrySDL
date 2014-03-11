#include "GraphicsDevice.h"

GraphicsDevice::GraphicsDevice() {
	screen = NULL;
}

void GraphicsDevice::Init()
{
	//SDL_Init(SDL_INIT_EVERYTHING);
	//TTF_Init();
	screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 32, SDL_SWSURFACE);
	pixels = (Uint32*)screen->pixels;

	//LoadModel("Teddybear.obj");
}

void GraphicsDevice::Draw() {
	SDL_Flip(screen);
}

void GraphicsDevice::Clear() {
	SDL_FillRect(screen, NULL, 0);
}

void GraphicsDevice::DrawLine(Vector3 start, Vector3 end) {
	//float dist = sqrt( (end.x - start.x) * (end.x - start.x) + (end.y - start.y)*(end.y - start.y));
	float dx = abs(start.x - end.x);
	float dy = abs(start.y - end.y);
	if (dx == 0 && dy == 0)
		return;
	float dt;
	bool horizontal;
	if (dx > dy) {
		dt = 1.0f / (dx + 1);
		horizontal = true;
	}
	else {
		dt = 1.0f / (dy + 1);
		horizontal = false;
	}

	for (float t = 0; t <= 1; t += dt) {
		//int x = (x0 - x1) * t + x0;
		float x = (end.x - start.x) * t + start.x;
		//int y = (y0 - y1) * t + y0;
		float y = (end.y - start.y) * t + start.y;
		Pixel pixel;
		pixel.pos.x = x;
		pixel.pos.y = y;
		float perc = 1.0f;
		if (horizontal) {
			if (y - (int)y > 0.0f) {
				perc -= y - (float)(int)y;
				Pixel pixel2;
				pixel2.pos.x = x;
				pixel2.pos.y = y + 1;
				pixel2.color = MAKE_COLOR((int)(255 * (1 - perc)), 0, 0, 0);
				DrawPixel(pixel);
			}
		}
		else {
			if (x - (float)(int)x > 0.0f) {
				perc -= x - (float)(int)x;
				Pixel pixel2;
				pixel2.pos.x = x + 1;
				pixel2.pos.y = y;
				pixel2.color = MAKE_COLOR((int)(255 * (1 - perc)), 0, 0, 0);
				DrawPixel(pixel);
			}
		}
		pixel.color = MAKE_COLOR((int)(255 * perc), 0, 0, 0);
		DrawPixel(pixel);
	}
}

void GraphicsDevice::DrawPixel(Pixel pixel) {
	if (pixel.pos.x < 0 || pixel.pos.x > SCREENWIDTH - 1)
		return;
	if (pixel.pos.y < 0 || pixel.pos.y > SCREENHEIGHT - 1)
		return;
	pixels[((int)pixel.pos.y*SCREENWIDTH + (int)pixel.pos.x)] = pixel.color;
}