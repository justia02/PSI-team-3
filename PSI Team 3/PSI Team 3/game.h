#pragma once

#include <irrlicht.h>
#include "mapterrain.h"
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include "BaseUnit.h"

#include "Player.h"
#include "PlayerCamera.h"
#include "GameStateDTO.h"
#include <stdlib.h>

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
	PlayerCamera* playerCamera;
	GameStateDTO* gameState;
	IGUIFont* unitModeLabel;
	std::wstring* unitModeLabelText;

	// private methods
	BaseUnit* initializeUnits();

	void updateGameState();

public:
	// constructor/desctructor
	game(void);
	game(IrrlichtDevice* device, ISceneManager* smgr, IGUIEnvironment* guienv);
	~game(void);

	// public methods
	int run(void);
	void startGame(bool asPlayer1, char* ipAddress = NULL); 
	void game::init_map(IrrlichtDevice *device_map);
	void startGame();
	void passTurn(bool giveUp);
	bool checkVictory(); 
	void resetGame();

	// players
	Player* localPlayer;
	Player* opposingPlayer;

};