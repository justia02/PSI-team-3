#include "menu.h"
#include <iostream>
#include "MenuEventReceiver.h"
#include <non-realtime-networking\NonRealtimeNetworkingException.h>

//class definition for menu
using namespace irrlicht_nonrealtimenetworking;

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;
	this->networkUtilities = new NonRealtimeNetworkingUtilities("145.109.160.20");

	//init();
}

menu::~menu(void)
{
}

void menu::run(/*IrrlichtDevice* device*/)
{

	// setup menu
	SAppContext context;
	context.device = device;
	context.counter = 0;
	//context.networkUtilities = netwo

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
			networkUtilities->hostGame(portNo);
			break;
		} else if (option == 2) {
			std::cout << "Please enter the IP address of your opponent" << std::endl;
			std::cin >> ipAddress;
			networkUtilities->joinGame(ipAddress, portNo);
			break;
		} else if (option == 3) {
			try {
				std::cout << networkUtilities->establishConnection("TEST", portNo) << std::endl;
				networkUtilities->receiveData();
				std::cout << "Received: " << networkUtilities->getBuffer() << std::endl;
			}
			catch (NonRealtimeNetworkingException e) {
				std::cout << "ERROR: " << e.what() << std::endl;
			}
			catch (std::exception e) {
				std::cout << "ERROR: " << e.what() << std::endl;
			}
			break;
		} else if (option == 4) {
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