#include "MenuEventReceiver.h"

//Event receiver custom class where we will later call the function for joining and creating a game
bool MenuEventReceiver::OnEvent(const SEvent& event)
{
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* guienv = Context.device->getGUIEnvironment();

            switch(event.GUIEvent.EventType)
            {
				 
				case EGET_BUTTON_CLICKED:
					switch(id)
					{
					case GUI_ID_JOIN_GAME:
						printf ("Join Game Button was clicked.");
						guienv->clear();
						text = new wchar_t[100];
						guienv->addEditBox(text, rect<s32>(160,25,480,50), true);
						//guienv->addStaticText();
						//guienv->addButton();
						return true;

					case GUI_ID_JOIN_GAME_DONE:
						guienv->clear();
						//char ch;
						//WideCharToMultiByte(CP_ACP, text, 
						Context.game_->startGame(false, "PLEASE FILL IN THE CORRECT IP HERE");
						return true;

					case GUI_ID_HOST_GAME:
						printf ("Host Game Button was clicked.");
						guienv->clear();
						Context.game_->startGame(true, ""); // call without ip, since we want to host 
						return true;


					case GUI_ID_JOIN_WSDL:

						return true;

					case GUI_ID_START_GAME:
						guienv->clear();
						Context.game_->init_map(Context.device);
						Context.game_->localPlayer->setPlayer1(true);
						Context.game_->opposingPlayer->setPlayer1(false);
						Context.game_->localPlayer->initUnits();
						Context.game_->opposingPlayer->initUnits();
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