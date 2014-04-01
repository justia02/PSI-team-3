#include "menu.h"
#include "MenuEventReceiver.h"
//class definition for menu
using namespace irrlicht_nonrealtimenetworking;

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;
	//this->networkUtilities = new NonRealtimeNetworkingUtilities("localhost");

	//init();
}

menu::~menu(void)
{
}

void menu::run(game* g) {

	// setup menu
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.game_ = g;

	/*
	// setup event receiver to handle user input on menu            
	MenuEventReceiver receiver(context);

	// specify our custom event receiver in the device	
	device->setEventReceiver(&receiver);
	*/

	// temporary console menu
	// portNo in our game
	int portNo = 6;
	int option;
	char* ipAddress = new char[15];

	while (true) {
		std::cout << "welcome! would you like to host(1) a game or join(2) a game, connect(3) to the Web Service or just start(4)?";
		std::cin >> option;

		if (option == 1) {
			context.game_->startGame(true, "");
//			networkUtilities->hostGame(portNo);
			break;
		} else if (option == 2) {
			std::cout << "Please enter the IP address of your opponent" << std::endl;
			std::cin >> ipAddress;
			context.game_->startGame(false, ipAddress);
//			networkUtilities->joinGame(ipAddress, portNo);
			break;
		} else if (option == 3) {
//			networkUtilities->establishConnection("TEST", portNo);
//			context.game_->startGame(true, "");
			break;
		} else if (option == 4) {
			context.game_->localPlayer->setPlayer1(true);
			context.game_->opposingPlayer->setPlayer1(false);
			context.game_->localPlayer->initUnits();
			context.game_->opposingPlayer->initUnits();

			break;
		}
	}
}

void menu::init(void)
{
	// add gui elements
	wchar_t* text2 = new wchar_t[100];
	guienv->addEditBox(text2,rect<s32>(160,300, 480,325 + 32),true);

	guienv->addButton(rect<s32>(160,100,480,150 + 32), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");

	text = L"add text here";
	guienv->addStaticText(text, rect<s32>(160,25,480,50), true);
}