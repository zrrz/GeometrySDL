#include "gameobject.h"
#include "SDL_image.h"

GameObject::GameObject()
{
	sprite = NULL;
	pos.x = 0; pos.y = 0;
	vel.x = 0; vel.y = 0;
	level = 1;

	prev_time  = 0;
	delta_time = 0;
}
GameObject::~GameObject()
{
	if ( sprite != NULL)
		SDL_FreeSurface( sprite );
	sprite = NULL;
}

void GameObject::loadimage(std::string filename)
{
	sprite = IMG_Load( filename.c_str() );
	
	//error check, make sure file loaded
	if ( sprite == NULL )
	{
		//404: image not found 
		exit(404);
	}

}
void GameObject::Update()
{
	float curr_time = SDL_GetTicks() / 1000.0f;
	delta_time      = curr_time - prev_time;
	prev_time       = curr_time;

	pos.x += vel.x * delta_time;
	pos.y += vel.y * delta_time;
}

void GameObject::Render(SDL_Surface* screen)
{
	offset.x = (int)pos.x;
	offset.y = (int)pos.y;

	SDL_BlitSurface(sprite, NULL, screen, &offset);

}

float GameObject::GetWidth()
{
	return sprite->w;
}

float GameObject::GetHeight()
{
	return sprite->h;
}

void GameObject::Collision(GameObject* other)
{

}