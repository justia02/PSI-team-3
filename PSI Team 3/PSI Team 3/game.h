#pragma once

#include <irrlicht.h>
#include "mapterrain.h"
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include "BaseUnit.h"

#include "Player.h"
#include "PlayerCamera.h"
#include "GameStateDTO.h"
#include "menu.h"
#include <stdlib.h>
#include <pthread.h>

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
	struct threadparam{
		unsigned int ipAdress;
		NonRealtimeNetworkingUtilities* networkUtils;
	};
	// members
	NonRealtimeNetworkingUtilities* networkUtilities;
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	PlayerCamera* playerCamera;
	
	IGUIFont* unitModeLabel;
	std::wstring* unitModeLabelText;
	bool endOfGame;
	bool firstTime;
	static bool connecting;
	pthread_t thread;
	pthread_t connectThread;
	void getFieldsList();

	// private methods
	BaseUnit* initializeUnits();

	static void * updateGameState(void * g);
	//void updateGameState();
	
	bool asPlayer1;
	char* ipAddress;
	std::vector<Obstacle*>* obstacles;
	std::vector<vector3df>* fields;

public:
	// constructor/desctructor
	game(void);
	game(IrrlichtDevice* device, ISceneManager* smgr, IGUIEnvironment* guienv);
	~game(void);

	GameStateDTO* gameState;

	// public methods
	int run(void);
	void startGame(bool asPlayer1, char* ipAddress = NULL); 
	//static void * startGame(void * g); 
	//void connect(bool asPlayer1, char* ipAddress = NULL);

	void init_ingame_menu();
	void init_map(IrrlichtDevice *device_map, std::vector<Obstacle*>* obstacles);

	void startGame();
	void passTurn(bool giveUp);
	bool checkVictory(); 
	void resetGame();
	bool getEndOfGame() { return endOfGame; };
	void setEndOfGame(bool endOfGame) { this->endOfGame = endOfGame; };


	int horizontal;
	int vertical;

	// players
	Player* localPlayer;
	Player* opposingPlayer;

	//menu
	menu* m;

	const wchar_t *names[6];
	const wchar_t *sub_names[6];
	//std::string names[6];
	//std::string sub_names[6];

	void init_names();

	rect<s32> makeButton(int width, int height, int i);

};