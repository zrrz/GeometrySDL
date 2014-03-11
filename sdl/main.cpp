#include "manager.h"
#include <string>


int main( int argc, char* args[] )
{
	Manager game;
	game.Init();

	while( game.Update() )	{ }
    return 0;
}