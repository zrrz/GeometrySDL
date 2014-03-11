#include <string>
#include "SDL.h"
#include "SDL_image.h"

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_


struct Vector2
{
	float x;
	float y;
	static float sqdist(const Vector2& a, const Vector2& b)
	{
		return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
	}
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void loadimage(std::string filename);
	virtual void Update();
	virtual void Render(SDL_Surface* screen);

	virtual void Collision(GameObject* other);

	virtual float GetWidth();
	virtual float GetHeight();

	Vector2 pos;
	Vector2 vel;
	int level; 

	SDL_Surface* sprite;
	SDL_Rect offset;

protected:
	float prev_time;
	float delta_time;

};

#endif