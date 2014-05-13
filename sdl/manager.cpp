#include "manager.h"
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
	//TTF_Init();
	
	graphicsDevice.Init();

	// "Tri.obj"
	// "Teddybear.obj"
	// "Robot.obj"

	LoadModel(&model, "Teddybear.obj");
	
	model.Scale(Vector3(11.0f, 11.0f, 11.0f));
	model.Rotate(Vector3(0.0f, 0.0f, 180.0f));
	model.Translate(Vector3(SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f, 0));
	//model.Translate(Vector3(0, 150.0f, 0));

	//camera.UpdateMatrix();
}

bool Manager::Update()
{
	if ( !KeyUpdate() )
		return false;

	graphicsDevice.Clear();

	model.Rotate(Vector3(0.0f, 0.1f, 0.0f));
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
		model.showWire = !model.showWire;
		break;
	case 'f':
		model.showFaces = !model.showFaces;
		break;
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