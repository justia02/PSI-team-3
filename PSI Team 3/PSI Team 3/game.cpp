#include "game.h"



game::game(void)
{
	device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3 - Checkers Game");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );


	//camera 
	   smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));


	//device->drop();
}


game::~game(void)
{
}

int game::run(void)
{
		while (device->run())
		{

			//device->run();
			driver->beginScene(true, true, SColor(255,100,101,140));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
		}

		return 1;
	
}
