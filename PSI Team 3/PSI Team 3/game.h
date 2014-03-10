#pragma once
#include <irrlicht.h>
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

class game
{
private:
	NonRealtimeNetworkingUtilities* networkUtilities;
	GameStateDTO* gameStateDTO;
	void hostGame();
	void joinGame(char* ipAddress);
	BaseUnit* initializeUnits();
	void passTurn();
	void receiveGameState();
	void deserialize();
public:

	IrrlichtDevice* device;

	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;

	game(void);
	~game(void);

	int run(void);
};

