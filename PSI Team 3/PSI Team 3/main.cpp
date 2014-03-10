#include <irrlicht.h>
#include <IGUIEnvironment.h>
#include <NonRealtimeNetworkingUtilities.h>
#include <BaseUnit.h>
#include <GameStateDTO.h>

using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace irrlicht_nonrealtimenetworking;

bool server;
char ipAddress[15];

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main ()
{

	server = true;
	strcpy(ipAddress, "127.0.0.1");

	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities();
	if (server) {
		utilities->setPortNumber(8);
		utilities->openServerSocket();
		utilities->acceptClient();
	}
	else {
		utilities->openClientSocket(ipAddress);
	}

	BaseUnit unit1 = BaseUnit(vector3d<float>(1.0f, 0.0f, 2.0f));
	BaseUnit unit2 = BaseUnit(vector3d<float>(2.0f, 1.0f, -1.0f));
	unit1.player1 = true;
	unit2.player1 = true;

	BaseUnit unit3 = BaseUnit(vector3d<float>(3.0f, 2.0f, -2.0f));
	BaseUnit unit4 = BaseUnit(vector3d<float>(4.0f, 2.0f, -3.0f));
	unit3.player1 = false;
	unit4.player1 = false;

	GameStateDTO* gameStateDTO = new GameStateDTO(4); // 4 - number of units
	BaseUnit units[4] = { unit1, unit2, unit3, unit4 };
	gameStateDTO->setUnits(units);

	std::ofstream ofs("gameStateDTO");

    // save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << gameStateDTO;
    	// archive and stream closed when destructors are called
    }

	std::ifstream ifs("gameStateDTO");

	std::filebuf* pbuf = ifs.rdbuf();

	// get file size using buffer's members
	size_t size = pbuf->pubseekoff (0, ifs.end, ifs.in);
	pbuf->pubseekpos (0, ifs.in);

	// allocate memory to contain file data
	char* buffer = new char[size];

	// get file data
	pbuf->sgetn(buffer, size);
	ifs.close();

	utilities->setBuffer(buffer); // Buffer ready to be sent

	IrrlichtDevice *device =
	createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"PSI TEAM 3 - Checkers Game");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//IGUIEditBox* editBox = new irr::gui::IGUIEnvironment::addEditBox(NULL, core::rect<s32>(), true, guienv->getRootGUIElement(), 0);	
	IGUIEditBox* editBox = new IGUIEnvironment::addEditBox("Hi", core::rect<s32>(), true, guienv->getRootGUIElement(), 0);

	irr::gui::IGUIElement* root = guienv->getRootGUIElement();
    const wchar_t* Text;
    Text = root->getElementFromId(0, true)->getText();
    char Word[256];
    for( int i = 0; i < sizeof( Text ); i++ )
    {
        char Chara = Text[i];
        Word[i] = Chara;
    }

	//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );



	//camera 
	smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));

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

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}


	device->drop();

	return 0;


}


