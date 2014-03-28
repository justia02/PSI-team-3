#pragma once

#include <irrlicht.h>
#include "mapterrain.h"
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include "BaseUnit.h"

#include "Player.h"
#include "PlayerCamera.h"
//#include "GameStateDTO.h"

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

	static const int portNumber = 8;

	// members
	NonRealtimeNetworkingUtilities* networkUtilities;
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	Player* localPlayer;
	Player* opposingPlayer;
	PlayerCamera* playerCamera;

	// private methods
	BaseUnit* initializeUnits();
	void passTurn();
	void receiveGameState();
	void deserialize();
	void updateGameState();

public:
	// constructor/desctructor
	game(void);
	game(IrrlichtDevice* device, ISceneManager* smgr, IGUIEnvironment* guienv);
	~game(void);

	// public methods
	int run(void);
	void startGame(bool asPlayer1, char* ipAddress); 
};