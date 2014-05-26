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

	// for gui events - from message boxes mainly
	if(event.EventType == EET_GUI_EVENT) {
		switch (event.GUIEvent.EventType) {
			// message boxes only appear when game has been ended in one way or another - end it in that case
			case EGET_MESSAGEBOX_OK: 
				if (Context.game_->getEndOfGame()) {
					Context.game_->resetGame();
				}
				break;
			default: 
				break;
		}
	}

	// if there is a key input and there is a unit selected
	if(event.EventType == EET_KEY_INPUT_EVENT)
	{	
		if((Context.game_->gameState->getPlayer1Turn() && Context.game_->localPlayer->getPlayer1()) || (!Context.game_->gameState->getPlayer1Turn()
			&& !Context.game_->localPlayer->getPlayer1())) {
				cout<<"IT IS YOUR TURN \n";
			if (this->isUnitSelected && event.KeyInput.Key != irr::KEY_KEY_P && event.KeyInput.Key != irr::KEY_SPACE) {
				// Create a list of all units if such doesn't exist yet
				if (allUnits == NULL) {
					allUnits = new std::vector<BaseUnit*>();
					allUnits->insert(allUnits->end(), unitList->begin(), unitList->end());
					allUnits->insert(allUnits->end(), Context.game_->opposingPlayer->getUnits()->begin(), Context.game_->opposingPlayer->getUnits()->end());
				}

				// set the direction from the keycode
				setDirection(event.KeyInput.Key);

				// Space was pressed - we're shooting :D
				if (shootingMode == true && Context.game_->localPlayer->actionAllowed() && !this->selectedUnit->getHasShot()) {
					this->selectedUnit->shoot(moveDirection, Context.game_->opposingPlayer->getUnits(), obstacles);
					Context.game_->localPlayer->setActionsLeft();
				}
				else if (shootingMode == false && Context.game_->localPlayer->actionAllowed() && !this->selectedUnit->getHasMoved()) { // Moving
					this->selectedUnit->Move(moveDirection, this->selectedUnit->maxDistance, allUnits, obstacles, Context.game_->localPlayer->getPlayer1());
					Context.game_->localPlayer->setActionsLeft();
				}
				//if (shootingMode == true) {
				//this->selectedUnit->shoot(moveDirection, Context.game_->opposingPlayer->getUnits(), obstacles);
				//}
				//else { // Moving
				//	this->selectedUnit->Move(moveDirection, this->selectedUnit->maxDistance, allUnits, obstacles, Context.game_->localPlayer->getPlayer1());
				//}

				this->selectedUnit->SelectUnit();
				this->selectedUnit = NULL;
				this->isUnitSelected = false;

			}
			else {

				if(event.KeyInput.Key == irr::KEY_KEY_P) {
					// block the turn if not your turn!
						time_t tmp = time(0); 
						if (tmp - timeSincePassTurn > 1) {
							timeSincePassTurn = tmp;
							Context.game_->passTurn(false);
						}	
				}
				if(event.KeyInput.Key == irr::KEY_KEY_G) {
					// block the turn if it is NOT your turn!
						Context.game_->passTurn(true);
				}
				if (event.KeyInput.PressedDown == true && event.KeyInput.Key == irr::KEY_SPACE) {
					shootingMode = !shootingMode;
					Context.game_->m->setModeText(((shootingMode == true) ? "Shooting mode" : "Moving Mode"));
					return true;
				}
			}
		} else {
			cout<<"NOT YOUR TURN";
			cout<<"\n";
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
				if (selectedUnit != NULL) {
					if (hoveredUnit == selectedUnit) { // we need to deselect the unit and return
						isUnitSelected = false;
						selectedUnit->SelectUnit();
						selectedUnit = NULL;
						Context.game_->m->setUnitText("Click on a unit to see his stats");
						return true;
					}
					else { // Deselect the unit which was selected and select another one
						selectedUnit->SelectUnit();
						selectedUnit = NULL;
						selectedUnit = hoveredUnit;
						selectedUnit->SelectUnit();
						Context.game_->m->setUnitText(getSelectedUnitData(selectedUnit));
						Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
						cout << "Select the direction you want to " << ((shootingMode == true) ? "shoot" : "move") << " (w,s,a,d)" << endl;
						return true;
					}						
				}
				else { // Select a unit
					selectedUnit = hoveredUnit;
					isUnitSelected = true;
					selectedUnit->SelectUnit();
					Context.game_->m->setUnitText(getSelectedUnitData(selectedUnit));
					Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
					cout << "Select the direction you want to " << ((shootingMode == true) ? "shoot" : "move") << " (w,s,a,d)" << endl;
				}
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

//get the information about the selected unit
std::string MenuEventReceiver::getSelectedUnitData(BaseUnit *unit)
{
	std::string data;
	std::string unittype;
	int distance = unit->maxDistance;
	int shooting = unit->shootingRange;

	std::string action;
	bool shot = unit->getHasShot();
	bool moved = unit->getHasMoved();

	//check for the unittype
	if (distance == 2)
	{
		unittype = "Balanced\n";
	}
	else if(distance == 3)
	{
		unittype = "Mover\n";
	}
	else{
		unittype = "Shooter\n";
	}

	//check for the action performed line
	if(!shot && !moved)
	{
		action = "This Unit has perfomed 0 actions\n";
	}
	else if(shot && moved)
	{
		action = "This Unit has moved and shot already!\n";
	}
	else if(!shot && moved)
	{
		action = "This Unit has moved already!\n";
	}
	else if(shot && !moved)
	{
		action = "This Unit has shot already!\n";
	}


	data =	"Unit Type = " + unittype +
			"Movement Range = " + std::string(std::to_string(static_cast<long double>(distance))) + "\n" +
			"Shooting Range = " + std::string(std::to_string(static_cast<long double>(shooting))) + "\n" +
		 action;

	return data;
}

/**
 * implementation of the 5 empty function in the menuReceiver in the engine
 */
void MenuEventReceiver::JOIN_GAME()
{
	// display interface to enter ip address
	text = L"Enter ip here";
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();					
	box = guienv->addEditBox(text, rect<s32>((Context.game_->horizontal / 3), (Context.game_->vertical/ 30) * 16, (Context.game_->horizontal / 3) * 2, (Context.game_->vertical/ 30) * 19), true);
	guienv->addButton(rect<s32>((Context.game_->horizontal / 3), (Context.game_->vertical/ 30) * 20, (Context.game_->horizontal / 3) * 2, (Context.game_->vertical/ 30) * 23), 0, GUI_ID_JOIN_GAME_SECOND, L"connect", L"connect to a game");
}

void MenuEventReceiver::HOST_GAME()
{
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->init_ingame_menu();
	Context.game_->connect(true, ""); // call without ip, since we want to host 
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

	Context.game_->connect(false, ch);
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->init_ingame_menu();
	menuDone = true;
}

void MenuEventReceiver::JOIN_WSDL()
{
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->init_ingame_menu();
	Context.game_->startGame();
	menuDone = true;
}

void MenuEventReceiver::START_GAME()
{
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->localPlayer->setPlayer1(true);
	Context.game_->opposingPlayer->setPlayer1(false);
	Context.game_->localPlayer->initUnits();
	Context.game_->opposingPlayer->initUnits();
	Context.game_->init_ingame_menu();
	Context.game_->localPlayer->setActionsLeft();

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
	line3d<f32> line;
	ISceneCollisionManager* collManager;
	ISceneNode *selectedNode;
	vector3df selectedPosition;

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
		if((*it)->node->getPosition() == selectedPosition)
		{
			this->isHoveringUnit = true;
			(*it)->highLightUnit(true);
			hoveredUnit = (BaseUnit*)(*it);

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
	Context.game_->m->setUnitText("Click on a unit to see his stats");
	Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
}
