#include "GraphicsDevice.h"

void Swap(void *, void *);

GraphicsDevice::GraphicsDevice() {
	screen = NULL;
}

void GraphicsDevice::Init()
{
	//SDL_Init(SDL_INIT_EVERYTHING);
	//TTF_Init();
	screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 32, SDL_SWSURFACE);
	pixels = (Uint32*)screen->pixels;
}

void GraphicsDevice::Draw() {
	SDL_Flip(screen);
}

void GraphicsDevice::Clear() {
	SDL_FillRect(screen, NULL, 0);
}

void GraphicsDevice::DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3) {

	if (v1.y > v2.y) Swap(&v1, &v2);
	if (v1.y > v3.y) Swap(&v1, &v3);
	if (v2.y > v3.y) Swap(&v2, &v3);
	
	long y1i = ceil(v1.y);
	long y2i = ceil(v2.y);
	long y3i = ceil(v3.y);

	if (y1i == y3i)
		return;

	float preStep;

	float dXdY_v1v3 = (v3.x - v1.x) / (v3.y - v1.y);
	float dXdY_v2v3 = (v3.x - v2.x) / (v3.y - v2.y);
	float dXdY_v1v2 = (v2.x - v1.x) / (v2.y - v1.y);

	bool mid = dXdY_v1v3<dXdY_v1v2;

	// if dXdY_V1V3 slope is bigger than dXdY_V1V2
	// then v2 is at the left side of triangle
	if (!mid) {
		// v2 is at the left side

		preStep = SUB_PIX(v1.y);

		Right_dXdY = dXdY_v1v3;

		if (y1i == y2i) {
			Left_dXdY = dXdY_v2v3;
			LeftX = v2.x + SUB_PIX(v2.y)*Left_dXdY;
			RightX = v1.x + preStep*Right_dXdY;
			DrawSegment(y1i, y3i);
			return;
		}

		if (y1i < y2i) {
			Left_dXdY = dXdY_v1v2;
			LeftX = v1.x + preStep*Left_dXdY;
			RightX = v1.x + preStep*Right_dXdY;
			DrawSegment(y1i, y2i);
		}

		if (y2i < y3i) {
			Left_dXdY = dXdY_v2v3;
			LeftX = v2.x + SUB_PIX(v2.y)*Left_dXdY;
			DrawSegment(y2i, y3i);
		}
	}
	else if (mid) {
		// v2 is at the right side

		preStep = SUB_PIX(v1.y);

		Left_dXdY = dXdY_v1v3;

		if (y1i == y2i) {
			Right_dXdY = dXdY_v2v3;
			LeftX = v1.x + preStep*Left_dXdY;
			RightX = v2.x + SUB_PIX(v2.y)*Right_dXdY;
			DrawSegment(y1i, y3i);
			return;
		}

		if (y1i<y2i) {
			Right_dXdY = dXdY_v1v2;
			LeftX = v1.x + preStep*Left_dXdY;
			RightX = v1.x + preStep*Right_dXdY;
			DrawSegment(y1i, y2i);
		}

		if (y2i<y3i) {
			Right_dXdY = dXdY_v2v3;
			RightX = v2.x + SUB_PIX(v2.y)*Right_dXdY;
			DrawSegment(y2i, y3i);
		}
	}
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

void GraphicsDevice::DrawSegment(long y1, long y2) {
	for (long y = y1; y < y2; y++) {
		long x1 = ceil(LeftX);
		long x2 = ceil(RightX);

		DrawLine(Vector3(x1, y, 1), Vector3(x2, y, 1));
		LeftX += Left_dXdY;
		RightX += Right_dXdY;
	}
}

void GraphicsDevice::Swap(Vector3 * a, Vector3 * b) {
	Vector3 temp = *a;
	*a = *b;
	*b = temp;
}