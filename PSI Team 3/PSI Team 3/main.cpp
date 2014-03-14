#include <irrlicht.h>
#include "PlayerCamera.h"
#include <vector3d.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main ()
{
	PlayerCamera test = PlayerCamera();
	vector3d<float> testpos = vector3d<float>(0.0, 0.0, 0.0);
	IrrlichtDevice *device =
	createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"PSI TEAM 3 - Checkers Game");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );



	//camera 
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));

	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, SColor(255,100,101,140));
		test.setCameraPos(device, testpos);
		if(testpos.Z < 4 && testpos.X < 4){
			testpos.Z += 0.002;
			//testpos.X += 0.0025;
		}
		else {
			testpos.Z = -4;
			//testpos.X = -4;
		}
		//test.setCameraPos(device, vector3d<float>(-10.0, 7.0, 0.0));
		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}


	device->drop();

	return 0;


}


