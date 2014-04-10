#include "MenuEventReceiver.h"
#include <assert.h>

//Event receiver custom class where we will later call the function for joining and creating a game
bool MenuEventReceiver::OnEvent(const SEvent& event)
{
	// if there is a key input and there is a unit selected
	if(event.EventType == EET_KEY_INPUT_EVENT && this->isUnitSelected)
	{
		// set the direction from the keycode
		setDirection(event.KeyInput.Key);
		// move the unit
		this->selectedUnit->Move(moveDirection, 2);
		// deselect the unit
		this->selectedUnit->SelectUnit();
		this->selectedUnit = NULL;
		this->isUnitSelected = false;
	}

	if(event.EventType == EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.Key == irr::KEY_KEY_P)
		{
			Context.game_->passTurn();
		}

	}
	// Remember the mouse state
    if (event.EventType == EET_MOUSE_INPUT_EVENT)
    {
        switch(event.MouseInput.Event)
        {
        case EMIE_LMOUSE_PRESSED_DOWN:
			// if the player's mouse is over a unit to select it
			if(this->isHoveringUnit)
			{
				//if(this->selectedUnit->selected)

				this->isUnitSelected = true;
				this->selectedUnit->SelectUnit();
				cout << "Select the direction you want to move (w,s,a,d)"<< endl;
			}
            break;
		case EMIE_MOUSE_MOVED:
			// check if the player is ready to select a unit
			MouseOverUnit();
			break;
		}
	}

	// logic for the menu
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

// if the player moves its mouse check if its over a frendly unit he can select
void MenuEventReceiver::MouseOverUnit()
{
	position2d<irr::s32> cursorPos;
	vector3df selectedPosition;
	line3d<f32> line;
	ISceneCollisionManager* collManager;
	ISceneNode *selectedNode;

	vector<BaseUnit*>::iterator it = this->unitList->begin();
	// get the cursor position
	cursorPos = this->Context.device->getCursorControl()->getPosition();
	// create the collition manager
	collManager = smgr->getSceneCollisionManager();
	// make a "line" from the mouse position
	line = collManager->getRayFromScreenCoordinates(cursorPos, camera);
	// get the object the line is colliding with
	selectedNode = collManager->getSceneNodeFromRayBB(line);
	// get the position of this "node"
	selectedPosition = selectedNode->getAbsolutePosition();
	// in the unitlist which unit was colliding the the mouse ray
	for(it ; it != unitList->end() ; ++it)
	{
		// deselect the unit
		(*it)->highLightUnit(false);
		//(*it)->selected = false;
		// if the positions of the node and the unit are the same
		// set this unit as the selected unit and highlight it
		if((*it)->position == selectedPosition)
		{
			this->isHoveringUnit = true;
			(*it)->highLightUnit(true);
			selectedUnit = (BaseUnit*)(*it);

			return;
		}
	}
	this->isHoveringUnit = false;
}

// sets the direction the unit should move in from the keycode
void MenuEventReceiver::setDirection(EKEY_CODE keyCode)
{
	// if there is no unit selected just return
	if(!this->isUnitSelected)
		return;
	// detect the keycode: if its a move direction key set the move direction
	if(keyCode == irr::KEY_KEY_W)
	{
		moveDirection = BaseUnit::FORWARD;
	}
	else if(keyCode == irr::KEY_KEY_S)
	{
		moveDirection = BaseUnit::BACK;
	}
	else if(keyCode == irr::KEY_KEY_A)
	{
		moveDirection = BaseUnit::LEFT;
	}
	else if(keyCode == irr::KEY_KEY_D)
	{
		moveDirection = BaseUnit::RIGHT;
	}
	// deselect the unit
	this->isUnitSelected = false;
}
