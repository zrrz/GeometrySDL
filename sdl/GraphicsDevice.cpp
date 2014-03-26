#include "GraphicsDevice.h"

#include <sstream>

#include <omp.h>

#define SUB_DIVIDE_SHIFT 4

#define SUB_DIVIDE_SIZE  (1<<SUB_DIVIDE_SHIFT)

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

	zBuffer = new Uint32[SCREENWIDTH*SCREENHEIGHT];
	ClearZBuffer();

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

	ClearZBuffer();

	SDL_Flip(screen);
}

void GraphicsDevice::ClearZBuffer() {
	for (int i = 0; i < SCREENWIDTH*SCREENHEIGHT; i++) {
		zBuffer[i] = 9999.0f;
	}
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

	float Right_dXdY, Left_dXdY;
	float LeftX, RightX;

	float Left_dZdY;
	float LeftZ;

	float z_a = 1.0 / v1.z;
	float z_b = 1.0 / v2.z;
	float z_c = 1.0 / v3.z;

	float denom = ((v3.x - v1.x) * (v2.y - v1.y)
		- (v2.x - v1.x) * (v3.y - v1.y));

	if (!denom) return;

	denom = 1.0f / denom;

	float PK_dzdx = ((z_c - z_a) * (v2.y - v1.y) - (z_b - z_a)
		* (v3.y - v1.y))*denom;

	PK_dzdx = PK_dzdx*SUB_DIVIDE_SIZE;

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

			Left_dZdY = dZdY_v2v3;
			LeftZ = z_b + SUB_PIX(v2.y)*Left_dZdY;

			DrawSegment(y1i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
			return;
		}
		else {
			if (y1i < y2i) {
				Left_dXdY = dXdY_v1v2;
				LeftX = v1.x + preStep*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY;

				Left_dZdY = dZdY_v1v2;
				LeftZ = z_a + preStep * Left_dZdY;
				
				DrawSegment(y1i, y2i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
			}

			if (y2i < y3i) {
				Left_dXdY = dXdY_v2v3;
				LeftX = v2.x + SUB_PIX(v2.y)*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY + (Right_dXdY*(y2i - y1i));

				Left_dZdY = dZdY_v2v3;
				LeftZ = z_b + SUB_PIX(v2.y)*Left_dZdY;

				DrawSegment(y2i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
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

			Left_dZdY = dZdY_v1v3;
			LeftZ = z_a + preStep*Left_dZdY;

			DrawSegment(y1i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
			return;
		}
		else {
			if (y1i < y2i) {
				Right_dXdY = dXdY_v1v2;
				LeftX = v1.x + preStep*Left_dXdY;
				RightX = v1.x + preStep*Right_dXdY;

				Left_dZdY = dZdY_v1v3;
				LeftZ = v1.x + preStep*Left_dZdY;

				DrawSegment(y1i, y2i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
			}

			if (y2i < y3i) {
				Right_dXdY = dXdY_v2v3;
				RightX = v2.x + SUB_PIX(v2.y)*Right_dXdY;
				LeftX = v1.x + preStep*Left_dXdY + (Left_dXdY*(y2i - y1i));

				Left_dZdY = dZdY_v1v3;
				LeftZ = v1.x + preStep*Left_dZdY;

				DrawSegment(y2i, y3i, color, Left_dXdY, Right_dXdY, LeftX, RightX, Left_dZdY, LeftZ, PK_dzdx);
			}
		}
	}
}

void GraphicsDevice::DrawLine(Vector3 start, Vector3 end, int color) {
	float dx = end.x - start.x;
	float dy = end.y - start.y;
	float dz = end.z - start.z;
	if (dx == 0 && dy == 0)
		return;
	float dt;

	if (abs(dx) > abs(dy)) {
		dt = 1.0f / (abs(dx) + 1.0f);
	}
	else {
		dt = 1.0f / (abs(dy) + 1.0f);
	}

	for (float t = 0; t <= 1; t += dt) {
		//int x = (x0 - x1) * t + x0;
		float x = dx * t + start.x;

		//int y = (y0 - y1) * t + y0;
		float y = dy * t + start.y;

		//int z = (z0 - z1) * t + z0;
		float z = dz * t + start.z;

		Pixel pixel;
		pixel.pos.x = x;
		pixel.pos.y = y;
		
		pixel.pos.z = z;

		pixel.color = color;
		DrawPixel(pixel);
	}
}

void GraphicsDevice::DrawPixel(Pixel pixel) {
	if (pixel.pos.x < 0 || pixel.pos.x > SCREENWIDTH - 1)
		return;
	if (pixel.pos.y < 0 || pixel.pos.y > SCREENHEIGHT - 1)
		return;
	if (zBuffer[(int)pixel.pos.y*SCREENWIDTH + (int)pixel.pos.x] < pixel.pos.z)
		return;
	zBuffer[(int)pixel.pos.y*SCREENWIDTH + (int)pixel.pos.x] = pixel.pos.z;
	pixels[((int)pixel.pos.y*SCREENWIDTH + (int)pixel.pos.x)] = pixel.color;
}

void GraphicsDevice::DrawSegment(long y1, long y2, int color, float Left_dXdY, float Right_dXdY, float LeftX, float RightX, float Left_dZdY, float LeftZ, float PK_dzdx) {
	
	float z;
	long x1, x2, y, x;
	
	for (y = y1; y < y2; y++) {
		x1 = ceil(LeftX);
		x2 = ceil(RightX);

		float width = x2 - x1;

		z = LeftZ + SUB_PIX(LeftX)*PK_dzdx;
		while (width >= SUB_DIVIDE_SIZE) {
			z += PK_dzdx;
			width -= SUB_DIVIDE_SIZE;
		}

		if (width > 0) {
			z += (PK_dzdx*width);
		}

		DrawLine(Vector3(x1, y, LeftZ), Vector3(x2, y, z), color);

		LeftX += Left_dXdY;
		RightX += Right_dXdY;

		LeftZ += Left_dZdY;
	}
}

void GraphicsDevice::Swap(Vector3 * a, Vector3 * b) {
	Vector3 temp = *a;
	*a = *b;
	*b = temp;
}