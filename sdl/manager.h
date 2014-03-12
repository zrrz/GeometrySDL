#include "SDL_ttf.h"		//used for text display

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <vector>
#include "Vectors.h"
#include "Model.h"
#include "GraphicsDevice.h"
#include "Camera.h"

//const int NUMOBJS = 10;

const int NUMKEYS = 7;
enum {UP, DOWN, LEFT, RIGHT, JUMP, DROP, CLICK};

class Manager
{
public:
	Manager();
	~Manager();

	void Init();
	bool Update();

	bool KeyUpdate();
	bool KeyDown(SDLKey key);
	bool KeyUp(SDLKey key);

	void LoadModel(Model* model, std::string);

	//void DrawModel();
private:
	
	GraphicsDevice graphicsDevice;

	Camera camera;

	Model model;
	
	//std::vector<Pixel> usedPixels;
	//Vector3 lineStart;
	Vector2 click;
	
	bool keys[NUMKEYS];

	//std::vector<Vector3> vectors;
	//std::vector<Face> faces;
};

#endif