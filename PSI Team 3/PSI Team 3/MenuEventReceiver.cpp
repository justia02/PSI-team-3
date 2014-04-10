#include "MenuEventReceiver.h"
#include <assert.h>

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
						text = L"Enter ip here";
						
						box = guienv->addEditBox(text, rect<s32>(160,25,480,50), true);
						//guienv->addStaticText();
						guienv->addButton(rect<s32>(160,200,480,250 + 32), 0, GUI_ID_JOIN_GAME_DONE, L"connect", L"connect to a game");
						return true;

					case GUI_ID_JOIN_GAME_DONE:
						const wchar_t* t;
						t = box->getText();
						size_t   i;
						ch = (char *)malloc(30);
						wcstombs_s(&i, ch, (size_t)30, t, (size_t)30 );

						printf(ch);
						guienv->clear();
						Context.game_->startGame(false, ch);
						Context.game_->init_map(Context.device);
						return true;

					case GUI_ID_HOST_GAME:
						printf ("Host Game Button was clicked.");
						guienv->clear();
						Context.game_->startGame(true, ""); // call without ip, since we want to host 
						Context.game_->init_map(Context.device);
						return true;


					case GUI_ID_JOIN_WSDL:
						guienv->clear();
						
						Context.game_->startGame();
						

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