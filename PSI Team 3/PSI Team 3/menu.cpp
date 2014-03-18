#include "menu.h"
#include <iostream>


// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
    IrrlichtDevice *device;
    s32             counter;
};

// Define values to identify join game and host game buttons
enum
{
    GUI_ID_JOIN_GAME,
    GUI_ID_HOST_GAME
};

void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
    for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(alpha);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver(SAppContext & context) : Context(context){ }

    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Context.device->getGUIEnvironment();

            switch(event.GUIEvent.EventType)
            {
				 
				case EGET_BUTTON_CLICKED:
					switch(id)
					{
					case GUI_ID_JOIN_GAME:
						printf ("Join Game Button was clicked.");
						return true;

					case GUI_ID_HOST_GAME:
						printf ("Host Game Button was clicked.");
						// Context.device->drop();
						return true;
					default:
						break;
					}
					break;

				default:
					break;
            }
        }

        return false; 
    }

	private:
    SAppContext & Context;
};

menu::menu(void)
{
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));
	//device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3");
	device->setResizable(false);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	 //wchar_t* text2 = new wchar_t[100];
	 //guienv->addEditBox(text2,rect<s32>(160,300, 480,325 + 32),true);

     guienv->addButton(rect<s32>(160,100,480,150 + 32), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	 guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");

	 text = L"add text here";
	 guienv->addStaticText(text, rect<s32>(160,25,480,50), true);

	 SAppContext context;
	 context.device = device;
     context.counter = 0;

	 menudone = false;

	 MyEventReceiver receiver(context);

     // And tell the device to use our custom event receiver.
     //device->setEventReceiver(&receiver);

	 networkUtilities = new NonRealtimeNetworkingUtilities();
}

void menu::hostGame() {
	networkUtilities->setPortNumber(8);
	networkUtilities->openServerSocket();
	networkUtilities->acceptClient();
}

void menu::joinGame(char* ipAddress) {
	networkUtilities->setPortNumber(8);
	networkUtilities->openClientSocket(ipAddress);
}

void menu::receiveData() {

	networkUtilities->receiveData();
	printf(networkUtilities->getBuffer());
}

void menu::sendData() {

	networkUtilities->setBuffer("HELLO");
	printf(networkUtilities->getBuffer());
	networkUtilities->sendData();
}

menu::~menu(void)
{
}

int menu::run(void)
{

	//if (menudone == false){
		init();
	//}
	//else{
		while (device->run() && driver)
		{
			 if (device->isWindowActive())
			 {

			//device->run();
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
			 }
		}
		//device->drop();
		
		
	//}
		

		// TODO launch map from here
	return 0;

}

void menu::init(void)
{
	// temporary console menu to establish connection
	
		int option;
		char* ipadress = new char[1];

		while (true) {
			std::cout << "welcome! would you like to host(1) a game or join(2) a game or quit(3)?";
			std::cin >> option;

			if (option == 1) {
				hostGame();
				menudone = true;
				break;

			} else if (option == 2) {

				std::cout << "Please enter the IP adress of your opponent";
				std::cin >> ipadress;
				joinGame(ipadress);
				menudone = true;
				break;
			} else if (option = 3) {
				menudone = true;
				break;
				//return 1;
			}
		}
		
		//return 0;

}