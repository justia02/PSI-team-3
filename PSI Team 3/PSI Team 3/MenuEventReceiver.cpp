#include "MenuEventReceiver.h"

//Event receiver custom class where we will later call the function for joining and creating a game
bool MenuEventReceiver::OnEvent(const SEvent& event)
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