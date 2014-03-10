#include <irrlicht.h>
#include "game.h" 

using namespace irr;



#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main ()
{

	game Game = game();
	Game.run();

}


