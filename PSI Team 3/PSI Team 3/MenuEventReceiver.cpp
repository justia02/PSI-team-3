#include "MenuEventReceiver.h"
#include <assert.h>

/**
 * implementation of event receiver class in the engine
 * it is a event receiver that is valid all over the game, not only in the menu
*/
bool MenuEventReceiver::OnEvent(const SEvent& event)
{
	//this is a call to the function in the base class where the program will check which button you press.
	menuSwitchCases(event, Context.device->getGUIEnvironment());

	// from here on all logic is about the game, not the menu
	// this will only be checked if the menu is done.
	if (!menuDone)
		return false;

	// if there is a key input and there is a unit selected
	if(event.EventType == EET_KEY_INPUT_EVENT && this->isUnitSelected)
	{
		// set the direction from the keycode
		setDirection(event.KeyInput.Key);

		// Create a list of all units if such doesn't exist yet
		//if (allUnits == NULL || allUnits->size() == 0) {
			allUnits = new std::vector<BaseUnit*>();
			allUnits->insert(allUnits->end(), unitList->begin(), unitList->end());
			allUnits->insert(allUnits->end(), Context.game_->opposingPlayer->getUnits()->begin(), Context.game_->opposingPlayer->getUnits()->end());
		//}

		// move
		this->selectedUnit->Move(moveDirection, 1, allUnits, Context.game_->localPlayer->getPlayer1());

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

    return false;
}

/**
 * implementation of the 5 empty function in the menuReceiver in the engine
 */
void MenuEventReceiver::JOIN_GAME()
{
	// display interface to enter ip address
	text = L"Enter ip here";
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();					
    box = guienv->addEditBox(text, rect<s32>(240,25,350,50), true);
	guienv->addButton(rect<s32>(240,100,350,150 + 10), 0, GUI_ID_JOIN_GAME_SECOND, L"connect", L"connect to a game");
}

void MenuEventReceiver::HOST_GAME()
{
	Context.game_->init_map(Context.device);
	Context.game_->startGame(true, ""); // call without ip, since we want to host 
	menuDone = true;
}

void MenuEventReceiver::JOIN_GAME_SECOND()
{
	const wchar_t* t;
	t = box->getText();
	size_t   i;
	ch = (char *)malloc(30);
	wcstombs_s(&i, ch, (size_t)30, t, (size_t)30 );

	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();

	Context.game_->startGame(false, ch);
	Context.game_->init_map(Context.device);
	menuDone = true;
}

void MenuEventReceiver::JOIN_WSDL()
{
	Context.game_->init_map(Context.device);
	Context.game_->startGame();
	menuDone = true;
}

void MenuEventReceiver::START_GAME()
{
	Context.game_->init_map(Context.device);
	Context.game_->localPlayer->setPlayer1(true);
	Context.game_->opposingPlayer->setPlayer1(false);
	Context.game_->localPlayer->initUnits();
	Context.game_->opposingPlayer->initUnits();

	menuDone = true;
}

/**
 * highlight friendly units on mouse over
 */
void MenuEventReceiver::MouseOverUnit()
{
	// this code is only relevant if the menu is already done
	if (!menuDone)
		return;
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

/**
 * move unit 
 */
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
