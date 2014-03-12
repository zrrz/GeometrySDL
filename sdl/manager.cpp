#include "manager.h"
#include <fstream>
#include <queue>
#include "math.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "Camera.h"
#include "GraphicsDevice.h"

Manager::Manager()
{	
	for (int i=0; i<NUMKEYS; i++)
		keys[i] = false;
}

Manager::~Manager()
{
	SDL_Quit();
}

void Manager::Init()
{
	SDL_Init( SDL_INIT_EVERYTHING );
	TTF_Init();
	
	graphicsDevice.Init();

	LoadModel(&model, "Teddybear.obj");
	
	model.Scale(Vector3(10.0f, 10.0f, 10.0f));
	model.Rotate(Vector3(0.0f, 0.0f, 180.0f));
	model.Translate(Vector3(SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f, 0));

	camera.UpdateMatrix();
}

bool Manager::Update()
{
	if ( !KeyUpdate() )
		return false;

	graphicsDevice.Clear();

	model.Rotate(Vector3(0.0f, 1.0f, 0.0f));
	model.Draw();

	graphicsDevice.Draw();

	//SDL_Delay( 1000/FPS );

	return true;
}

void Manager::LoadModel(Model* model, std::string fileName) {
	
	std::vector<Vector3> vectors;
	std::vector<Face> faces;
	
	std::ifstream file(fileName);

	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::stringstream stream(line);
			std::string type;
			stream >> type;
			if (type == "v") {
				Vector3 t_vector;
				float f;
				stream >> f;
				t_vector.x = f;
				stream >> f;
				t_vector.y = f;
				stream >> f;
				t_vector.z = f;
				vectors.push_back(t_vector);
			}
			else if (type == "f") {
				Face t_face;
				int num;
				stream >> num;
				t_face.a1 = num;
				stream >> num;
				t_face.b1 = num;
				stream >> num;
				t_face.c1 = num;
				faces.push_back(t_face);
			}
		}
		file.close();
	}
	model->LoadContent(faces, vectors, &graphicsDevice);
}

//void Manager::DrawModel() {
//	for (int i = 0, n = faces.size(); i < n; ++i) {
//		Matrix scale;
//		scale.m[0] = scale.m[5] = scale.m[10] = 10;
//
//		Matrix center;
//		center.m[3] = SCREENWIDTH/2;
//		center.m[7] = SCREENHEIGHT/2;
//
//		Matrix rotate;
//		float theta = 180 * (PI/180.0f);
//		//theta += 0.1*(PI/180.0f);
//		rotate.m[0] = cos(theta);
//		rotate.m[1] = -(sin(theta));
//		rotate.m[4] = sin(theta);
//		rotate.m[5] = cos(theta);
//
//		Matrix rotate2;
//		static float theta2 = 180 * (PI / 180.0f);
//		theta2 += 0.0005*(PI/180.0f);
//		rotate.m[0] = cos(theta2);
//		rotate.m[2] = sin(theta2);
//		rotate.m[8] = -(sin(theta2));
//		rotate.m[10] = cos(theta2);
//
//		Matrix allTransforms = center * scale * rotate;
//		//allTransforms = allTransforms * allTransforms.Inverse();
//
//		DrawLine(allTransforms * vectors[faces[i].a1 - 1], allTransforms * vectors[faces[i].b1 - 1]);
//		DrawLine(allTransforms * vectors[faces[i].a1 - 1], allTransforms * vectors[faces[i].c1 - 1]);
//		DrawLine(allTransforms * vectors[faces[i].b1 - 1], allTransforms * vectors[faces[i].c1 - 1]);
//	}
//}

bool Manager::KeyUpdate()
{
	SDL_Event keyevent;
	
	while ( SDL_PollEvent( &keyevent ) )
	{
		//click.x = keyevent.motion.x;
		//click.y = keyevent.motion.y;
		switch ( keyevent.type )
		{
		case SDL_KEYDOWN:
			if ( !KeyDown( keyevent.key.keysym.sym ) )
				return false;
			break;
		case SDL_KEYUP:
			if ( !KeyUp( keyevent.key.keysym.sym ) )
				return false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			keys[CLICK] = true;
			click.x = keyevent.motion.x;
			click.y = keyevent.motion.y;
			break;

		case SDL_MOUSEBUTTONUP:
			keys[CLICK] = false;
			break;
		/*
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		*/
		case SDL_QUIT:
			return false;
			break;
		}
	}

	return true;
}

bool Manager::KeyDown(SDLKey key)
{
	switch ( key )
	{
	case '[':
		break;
	case ']':
		break;
	case 'w':
	case SDLK_UP:
		keys[UP] = true;
		break;
	case 'a':
	case 't':
	case SDLK_LEFT:
		keys[LEFT] = true;
		break;
	case 's':
	case SDLK_DOWN:
		keys[DOWN] = true;
		break;
	case 'd':
	case SDLK_RIGHT:
		keys[RIGHT] = true;
		break;

	case SDLK_SPACE:
		keys[JUMP] = true;
		break;

	case SDLK_LSHIFT:
		keys[DROP] = true;
		break;


	case SDLK_ESCAPE:
		return false;
		break;
	}
	return true;
}

bool Manager::KeyUp(SDLKey key)
{
	switch ( key )
	{
	case 'w':
	case SDLK_UP:
		keys[UP] = false;
		break;
	case 'a':
	case SDLK_LEFT:
		keys[LEFT] = false;
		break;
	case 's':
	case SDLK_DOWN:
		keys[DOWN] = false;
		break;
	case 'd':
	case SDLK_RIGHT:
		keys[RIGHT] = false;
		break;

	case SDLK_SPACE:
		keys[JUMP] = false;
		break;

	case SDLK_LSHIFT:
		keys[DROP] = false;
		break;

	case SDLK_ESCAPE:
		return false;
		break;
	}
	return true;
}
