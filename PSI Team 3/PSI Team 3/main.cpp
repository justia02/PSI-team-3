#include <irrlicht.h>
#include "menu.h" 

using namespace irr;



#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main ()
{
	//test
	menu Game = menu();
	Game.run();

}


