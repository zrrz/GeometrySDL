#include "GraphicsDevice.h"

#include <sstream>

#include <omp.h>

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

	txt_img = NULL;
	txt_color.r = 255; 
	txt_color.g = txt_color.b = 0;
	txt_pos.x = txt_pos.y = 10;

	TTF_Init();
	font = TTF_OpenFont("fonts/arial.ttf", 24);
}

void GraphicsDevice::Draw() {
	Uint32 now = SDL_GetTicks();
	Uint32 dt = now - last;
	last = now;

	std::ostringstream out;
	out << 1000.0f / dt;

	txt = "FPS: " + out.str();
	txt_img = TTF_RenderText_Solid(font, txt.c_str(), txt_color);
	SDL_BlitSurface(txt_img, 0, screen, &txt_pos);
	SDL_FreeSurface(txt_img);

	SDL_Flip(screen);
}

void GraphicsDevice::Clear() {
	SDL_FillRect(screen, NULL, 0);
}

void GraphicsDevice::DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3) {

	Vector3 lightVector(1, -1, 0);
	lightVector = lightVector.Normalized();

	Vector3 normal = (v2 - v1).Cross(v3 - v2);
	normal = normal.Normalized();

	//Vector3 camDir(0.0f, 0.0f, 1.0f);
	//if (camDir*normal > 1.0f)
	//	return;

	//Vector3 camDir(0.0f, 0.0f, 1.0f);	
	//if (camDir*normal > 1.0f)
	//	return;

	float dot = lightVector * normal;
	if (dot<0) dot = 0;
	if (dot>1) dot = 1;

	int color = MAKE_COLOR(0, 0, (int)(255 * dot), 255);

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

	float dZdY_v1v3 = (v3.z - v1.z) / (v3.y - v1.y);
	float dZdY_v2v3 = (v3.z - v2.z)	/ (v3.y - v2.y);
	float dZdY_v1v2 = (v2.z - v1.z) / (v2.y - v1.y);

	bool mid = dXdY_v1v3<dXdY_v1v2;

	float Right_dXdY;
	float Left_dXdY;
	float Right_dZdY;
	float Left_dZdY;
	float LeftX, RightX;
	float LeftZ, RightZ;

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
			DrawSegment(y1i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			return;
		}
		else {

			if (y1i < y2i) {
				Left_dXdY = dXdY_v1v2;
				LeftX = v1.x + preStep*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY;
				
				DrawSegment(y1i, y2i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			}

			if (y2i < y3i) {
				Left_dXdY = dXdY_v2v3;
				LeftX = v2.x + SUB_PIX(v2.y)*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY + (Right_dXdY*(y2i - y1i));

				DrawSegment(y2i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			}
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
			DrawSegment(y1i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			return;
		}
		else {

			if (y1i < y2i) {
				Right_dXdY = dXdY_v1v2;
				LeftX = v1.x + preStep*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY;
				DrawSegment(y1i, y2i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			}

			if (y2i < y3i) {
				Right_dXdY = dXdY_v2v3;
				RightX = v2.x + SUB_PIX(v2.y)*Right_dXdY;
				LeftX = v1.x + preStep*Left_dXdY + (Left_dXdY*(y2i - y1i));
				DrawSegment(y2i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, Right_dZdY, LeftZ, RightZ);
			}
		}
	}
}

void GraphicsDevice::DrawLine(Vector3 start, Vector3 end, int color) {
	//float dist = sqrt( (end.x - start.x) * (end.x - start.x) + (end.y - start.y)*(end.y - start.y));
	float dx = abs(start.x - end.x);
	float dy = abs(start.y - end.y);
	if (dx == 0 && dy == 0)
		return;
	float dt;
	//bool horizontal;
	if (dx > dy) {
		dt = 1.0f / (dx + 1.0f);
	//	horizontal = true;
	}
	else {
		dt = 1.0f / (dy + 1.0f);
	//	horizontal = false;
	}

	for (float t = 0; t <= 1; t += dt) {
		//int x = (x0 - x1) * t + x0;
		float x = (end.x - start.x) * t + start.x;
		//int y = (y0 - y1) * t + y0;
		float y = (end.y - start.y) * t + start.y;
		Pixel pixel;
		pixel.pos.x = x;
		pixel.pos.y = y;
		//float perc = 1.0f;
		//if (horizontal) {
		//	if (y - (int)y > 0.0f) {
		//		perc -= y - (float)(int)y;
		//		Pixel pixel2;
		//		pixel2.pos.x = x;
		//		pixel2.pos.y = y + 1;
		//		pixel2.color = MAKE_COLOR((int)(255 * (1 - perc)), 0, 0, 0);
		//		DrawPixel(pixel);
		//	}
		//}
		//else {
		//	if (x - (float)(int)x > 0.0f) {
		//		perc -= x - (float)(int)x;
		//		Pixel pixel2;
		//		pixel2.pos.x = x + 1;
		//		pixel2.pos.y = y;
		//		pixel2.color = MAKE_COLOR((int)(255 * (1 - perc)), 0, 0, 0);
		//		DrawPixel(pixel);
		//	}
		//}
		pixel.color = color;
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

void GraphicsDevice::DrawSegment(long y1, long y2, int color, float Left_dXdY, float Right_dXdY, float LeftX, float RightX, float Left_dZdY, float Right_dZdY, float LeftZ, float RightZ) {
	for (long y = y1; y < y2; y++) {
		long x1 = ceil(LeftX);
		long x2 = ceil(RightX);

		//float zSlope = 1 / (RightZ - LeftZ);

		DrawLine(Vector3(x1, y, LeftZ), Vector3(x2, y, RightZ), color);
		LeftX += Left_dXdY;
		RightX += Right_dXdY;

		LeftZ += Left_dZdY;
		RightZ += Right_dZdY;
	}
}

void GraphicsDevice::Swap(Vector3 * a, Vector3 * b) {
	Vector3 temp = *a;
	*a = *b;
	*b = temp;
}