#include "menu.h"
#include <iostream>
#include "MenuEventReceiver.h";
#include "BaseUnit.h"

//class definition for menu
using namespace irrlicht_nonrealtimenetworking;

menu::menu(void)
{

	 init();

	 SAppContext context;
	 context.device = device;
     context.counter = 0;

	 MenuEventReceiver receiver = MenuEventReceiver(context);
	 //receiver.onEvent();

	//here we make the device
	//device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));
	device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3");
	device->setResizable(false);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	 //wchar_t* text2 = new wchar_t[100];
	 //guienv->addEditBox(text2,rect<s32>(160,300, 480,325 + 32),true);

     //guienv->addButton(rect<s32>(160,100,480,150 + 32), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	 //guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");

	 //text = L"add text here";
	 //guienv->addStaticText(text, rect<s32>(160,25,480,50), true);
	smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));

	 mapterrain map = mapterrain(device, smgr);

	 unit1 = new BaseUnit(vector3d<float>(0, 1, 0), device);
	 unit2 = new BaseUnit(vector3d<float>(2, 1, 2), device);

	 menudone = false;

	 

     // And tell the device to use our custom event receiver.
     //device->setEventReceiver(&receiver);

	 
}

menu::~menu(void)
{
}

int menu::run(void)
{

		while (device->run() && driver)
		{
			if (device->isWindowActive())
			{
			
			//device->run();
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
			unit1->Move(BaseUnit::direction::BACK ,0.01);
			}
		}
		
	return 0;

}

void menu::init(void)
{
	// temporary console menu to establish connection
	
		networkUtilities= new NonRealtimeNetworkingUtilities();
		// portNo is our game
		int portNo = 8;
		int option;
		char* ipadress = new char[1];

		while (true) {
			std::cout << "welcome! would you like to host(1) a game or join(2) a game or just start(3)?";
			std::cin >> option;

			if (option == 1) {
				networkUtilities->hostGame(portNo);
				menudone = true;
				break;

			} else if (option == 2) {

				std::cout << "Please enter the IP adress of your opponent";
				std::cin >> ipadress;
				networkUtilities->joinGame(ipadress, portNo);
				menudone = true;
				break;
			} else if (option = 3) {
				menudone = true;
				break;
			}
		}
}