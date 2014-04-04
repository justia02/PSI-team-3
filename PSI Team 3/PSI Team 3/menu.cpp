#include "menu.h"

//class definition for menu
using namespace irrlicht_nonrealtimenetworking;

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;
	//this->networkUtilities = new NonRealtimeNetworkingUtilities("localhost");

	init();
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

	//init_console(context);
	
	// setup event receiver to handle user input on menu            
	//MenuEventReceiver receiver(context);

	// specify our custom event receiver in the device	
	//device->setEventReceiver(&receiver);
	




}

void menu::init(void)
{
	// add gui elements
	//wchar_t* text2 = new wchar_t[100];
	//guienv->addEditBox(text2,rect<s32>(160,300, 480,325 + 32),true);

	guienv->addButton(rect<s32>(160,100,480,150 + 32), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");
	guienv->addButton(rect<s32>(160,300,480,350 + 32), 0, GUI_ID_JOIN_WSDL, L"Join Server", L"Joins the server that will assign players");
	guienv->addButton(rect<s32>(160,400,480,450 + 32), 0, GUI_ID_START_GAME, L"Start Game", L"Start game without network for test");

	//text = L"add text here";
	//guienv->addStaticText(text, rect<s32>(160,25,480,50), true);
}

void menu::init_console(SAppContext context)
{
		// temporary console menu
		// portNo in our game
		int portNo = 6;
		int option;
		char* ipAddress = new char[15];

		while (true) {
		std::cout << "welcome! would you like to host(1) a game or join(2) a game, connect(3) to the Web Service or just start(4)?";
		std::cin >> option;

		if (option == 1) {
			try {
				context.game_->startGame(true);
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
			break;
		} else if (option == 2) {
			std::cout << "Please enter the IP address of your opponent" << std::endl;
			std::cin >> ipAddress;
			try {
				context.game_->startGame(false, ipAddress);
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
			break;
		} else if (option == 3) {
			try {
				context.game_->startGame();
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
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