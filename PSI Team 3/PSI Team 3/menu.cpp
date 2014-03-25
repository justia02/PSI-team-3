#include "menu.h"
#include <iostream>
#include "MenuEventReceiver.h";

//class definition for menu
using namespace irrlicht_nonrealtimenetworking;

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv, NonRealtimeNetworkingUtilities* networkUtilities) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;
	//this->networkUtilities = networkUtilities;

	//init();



}

menu::~menu(void)
{
}

void menu::run(IrrlichtDevice* device)
{
/*
		// setup menu
	SAppContext context;
	context.device = device;
	context.counter = 0;
	//context.networkUtilities = netwo

	// setup event receiver to handle user input on menu            
	MenuEventReceiver receiver(context);

	// specify our custom event receiver in the device	
	device->setEventReceiver(&receiver);

	//return 0;
	*/
	
	// temporary console menu
	// portNo in our game
	int portNo = 8;
	int option;
	char* ipadress = new char[1];

	while (true) {
		std::cout << "welcome! would you like to host(1) a game or join(2) a game or just start(3)?";
		std::cin >> option;

		if (option == 1) {
			networkUtilities->hostGame(portNo);
			break;

		} else if (option == 2) {

			std::cout << "Please enter the IP adress of your opponent";
			std::cin >> ipadress;
			networkUtilities->joinGame(ipadress, portNo);
			break;
		} else if (option = 3) {
			break;
		}
	}
	//return 0;
	
}

void menu::init(void)
{

	

	//receiver.onEvent();

	// add gui elements
	wchar_t* text2 = new wchar_t[100];
	guienv->addEditBox(text2,rect<s32>(160,300, 480,325 + 32),true);

	guienv->addButton(rect<s32>(160,100,480,150 + 32), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");

	text = L"add text here";
	guienv->addStaticText(text, rect<s32>(160,25,480,50), true);


	
	

}