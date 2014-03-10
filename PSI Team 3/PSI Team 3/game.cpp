#include "game.h"

void game::hostGame() {
	networkUtilities->setPortNumber(8);
	networkUtilities->openServerSocket();
	networkUtilities->acceptClient();
}

void game::joinGame(char* ipAddress) {
	networkUtilities->openClientSocket(ipAddress);
}

BaseUnit* game::initializeUnits() {

	BaseUnit unit1 = BaseUnit(vector3d<float>(1.0f, 0.0f, 2.0f));
	BaseUnit unit2 = BaseUnit(vector3d<float>(2.0f, 1.0f, -1.0f));
	unit1.player1 = true;
	unit2.player1 = true;

	BaseUnit unit3 = BaseUnit(vector3d<float>(3.0f, 2.0f, -2.0f));
	BaseUnit unit4 = BaseUnit(vector3d<float>(4.0f, 2.0f, -3.0f));
	unit3.player1 = false;
	unit4.player1 = false;

	BaseUnit* units = new BaseUnit[4];
	units[0] = unit1;
	units[1] = unit2;
	units[2] = unit3;
	units[3] = unit4;
	return units;

}

void game::passTurn() {

	gameStateDTO->setPlayer1Turn(!gameStateDTO->getPlayer1Turn());
	networkUtilities->setBuffer(gameStateDTO->serialize());
	networkUtilities->sendData();

}

void game::receiveGameState() {

	networkUtilities->receiveData();
	deserialize();

}

game::game(void)
{

	networkUtilities = new NonRealtimeNetworkingUtilities();
	gameStateDTO = new GameStateDTO(4);
	gameStateDTO->setUnits(initializeUnits());

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
