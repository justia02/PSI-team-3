#include "MenuEventReceiver.h"
#include <assert.h>

/**
 * implementation of event receiver class in the engine
 * it is a event receiver that is valid all over the game, not only in the menu
*/
bool MenuEventReceiver::OnEvent(const SEvent& event)
{
	//this is a call to the function in the base class where the program will check which button you press.
	menuSwitchCases(event, Context.device->getGUIEnvironment(), (menuReceiver::JOIN_GAME)(&MenuEventReceiver::JOIN_GAME), 
																(menuReceiver::HOST_GAME)(&MenuEventReceiver::HOST_GAME), 
																(menuReceiver::JOIN_GAME_SECOND)(&MenuEventReceiver::JOIN_GAME_SECOND), 
																(menuReceiver::END_GAME)(&MenuEventReceiver::END_GAME));

	/*menuCustomCases_6(event, Context.device->getGUIEnvironment(),	(menuReceiver::menu1)(&MenuEventReceiver::menu1),
																	(menuReceiver::menu2)(&MenuEventReceiver::menu2),
																	(menuReceiver::menu3)(&MenuEventReceiver::menu3),
																	(menuReceiver::menu4)(&MenuEventReceiver::menu4),
																	(menuReceiver::menu5)(&MenuEventReceiver::menu5),
																	(menuReceiver::menu6)(&MenuEventReceiver::menu6)
		);*/
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
	}
    else if (event.EventType == EET_MOUSE_INPUT_EVENT) {

        switch(event.MouseInput.Event)
        {
        case EMIE_LMOUSE_PRESSED_DOWN:
			// Not our turn
			if(!(Context.game_->gameState->getPlayer1Turn() && Context.game_->localPlayer->getPlayer1()) && !(!Context.game_->gameState->getPlayer1Turn() && !Context.game_->localPlayer->getPlayer1()))
				return false;

			// if the player's mouse is over a unit to select it
			if(this->isHoveringUnit)
			{
				if (selectedUnit != NULL) {
					if (hoveredUnit == selectedUnit) { // we need to deselect the unit and return
						isUnitSelected = false;
						selectedUnit->SelectUnit();
						selectedUnit = NULL;
						clearPossibleMoves();
						clearHighlightedEnemies();
						Context.game_->m->setUnitText("Click on a unit to see its stats");
						return true;
					}
					else {
						if (unitToBeShot == NULL && hoveredUnit != NULL) { // Our unit
							// Deselect the unit which was selected and select another one
							selectedUnit->SelectUnit();
							selectedUnit = NULL;
							selectedUnit = hoveredUnit;
							selectedUnit->SelectUnit();
							clearPossibleMoves();
							clearHighlightedEnemies();
							if (Context.game_->localPlayer->actionAllowed()) {
								if (!selectedUnit->getHasMoved())
									checkPossibleMoves();
								if (!selectedUnit->getHasShot())
									checkOpponentsToShoot();
							}
							Context.game_->m->setUnitText(getSelectedUnitData(selectedUnit));						
							return true;
						}
						else if (unitToBeShot != NULL) { // Enemy unit hovered - let's shoot
							if (selectedUnit->canShoot(unitToBeShot->position)) {
								selectedUnit->attack(unitToBeShot);
								Context.game_->localPlayer->setActionsLeft();
								Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
								selectedUnit->SelectUnit();
								selectedUnit = NULL;
								clearPossibleMoves();
								clearHighlightedEnemies();
							}
							else
								std::cout << "Enemy not in your range!" << std::endl;

							return true;

						}
					}						
				}
				else if (hoveredUnit != NULL) { // Select a unit
					selectedUnit = hoveredUnit;
					isUnitSelected = true;
					selectedUnit->SelectUnit();
					clearPossibleMoves();
					clearHighlightedEnemies();
					if (Context.game_->localPlayer->actionAllowed()) {
						if (!selectedUnit->getHasMoved())
							checkPossibleMoves();
						if (!selectedUnit->getHasShot())
							checkOpponentsToShoot();
					}
					Context.game_->m->setUnitText(getSelectedUnitData(selectedUnit));
					return true;
				}
			}
			else {
				if (isUnitSelected) {
					// Moving
					if (hoveredGridNode != NULL) {
						selectedUnit->setPosition(hoveredGridNode->position);
						selectedUnit->setHasMoved(true);
						Context.game_->localPlayer->setActionsLeft();
						Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
						clearPossibleMoves();
						clearHighlightedEnemies();
						hoveredGridNode = NULL;

						// Deselect the unit
						isUnitSelected = false;
						selectedUnit->SelectUnit();
						selectedUnit = NULL;
					}
					else {
						std::cout << "YOU CANNOT MOVE THERE!!!" << std::endl;
					}

					return true;

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

void MenuEventReceiver::clearHighlightedEnemies() {

	for(vector<BaseUnit*>::iterator it = this->highlightedEnemies->begin(); it != highlightedEnemies->end() ; ++it)
		(*it)->highLightUnit(false);

	// Clear the list
	highlightedEnemies->clear();

}

void MenuEventReceiver::checkOpponentsToShoot() {

	BaseUnit* enemyUnit = NULL;

	enemyUnit = selectedUnit->canShoot(BaseUnit::direction::BACK, Context.game_->opposingPlayer->getUnits(), obstacles);
	if (enemyUnit != NULL) {
		enemyUnit->highlightShoot(true);
		highlightedEnemies->push_back(enemyUnit);
	}

	enemyUnit = selectedUnit->canShoot(BaseUnit::direction::FORWARD, Context.game_->opposingPlayer->getUnits(), obstacles);
	if (enemyUnit != NULL) {
		enemyUnit->highlightShoot(true);
		highlightedEnemies->push_back(enemyUnit);
	}

	enemyUnit = selectedUnit->canShoot(BaseUnit::direction::LEFT, Context.game_->opposingPlayer->getUnits(), obstacles);
	if (enemyUnit != NULL) {
		enemyUnit->highlightShoot(true);
		highlightedEnemies->push_back(enemyUnit);
	}

	enemyUnit = selectedUnit->canShoot(BaseUnit::direction::RIGHT, Context.game_->opposingPlayer->getUnits(), obstacles);
	if (enemyUnit != NULL) {
		enemyUnit->highlightShoot(true);
		highlightedEnemies->push_back(enemyUnit);
	}

}
void MenuEventReceiver::clearPossibleMoves() {

	// Remove each node separately
	for(vector<GridNode*>::iterator it = this->highlightedNodes->begin(); it != highlightedNodes->end() ; ++it)
		(*it)->node->remove();

	// Clear the list
	highlightedNodes->clear();

}

void MenuEventReceiver::checkPossibleMoves() {

	vector3df unitPosition = selectedUnit->position;
	int distance = selectedUnit->maxDistance;

	std::vector<vector3df> possibleMoves;

	int stepCounter = 1;
	moveDirection = BaseUnit::direction::BACK;
	while (selectedUnit->canMove(moveDirection, stepCounter, getAllUnits(), obstacles) == true && stepCounter <= distance) {
		possibleMoves.push_back(vector3df(unitPosition.X, unitPosition.Y, unitPosition.Z - stepCounter));
		stepCounter++;
	}

	stepCounter = 1;
	moveDirection = BaseUnit::direction::FORWARD;
	while (selectedUnit->canMove(moveDirection, stepCounter, getAllUnits(), obstacles) == true && stepCounter <= distance) {
		possibleMoves.push_back(vector3df(unitPosition.X, unitPosition.Y, unitPosition.Z + stepCounter));
		stepCounter++;
	}

	stepCounter = 1;
	moveDirection = BaseUnit::direction::LEFT;
	while (selectedUnit->canMove(moveDirection, stepCounter, getAllUnits(), obstacles) == true && stepCounter <= distance) {
		possibleMoves.push_back(vector3df(unitPosition.X - stepCounter, unitPosition.Y, unitPosition.Z));
		stepCounter++;
	}

	stepCounter = 1;
	moveDirection = BaseUnit::direction::RIGHT;
	while (selectedUnit->canMove(moveDirection, stepCounter, getAllUnits(), obstacles) == true && stepCounter <= distance) {
		possibleMoves.push_back(vector3df(unitPosition.X + stepCounter, unitPosition.Y, unitPosition.Z));
		stepCounter++;
	}

	if (stepCounter == 0) {
		std::cout << "This unit cannot move anywhere!!!" << std::endl;
		return;
	}

	// Mark availible fields on the map
	for (int i=0; i<possibleMoves.size(); i++) {
		highlightedNodes->push_back(new GridNode(Context.device->getSceneManager(), getMesh(), possibleMoves[i]));
	}

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
bool MenuEventReceiver::JOIN_GAME()
{
	// display interface to enter ip address
	text = L"Enter ip here";
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();					
	box = guienv->addEditBox(text, rect<s32>((Context.game_->horizontal / 3), (Context.game_->vertical/ 30) * 16, (Context.game_->horizontal / 3) * 2, (Context.game_->vertical/ 30) * 19), true);
	guienv->addButton(rect<s32>((Context.game_->horizontal / 3), (Context.game_->vertical/ 30) * 20, (Context.game_->horizontal / 3) * 2, (Context.game_->vertical/ 30) * 23), 0, GUI_ID_JOIN_GAME_SECOND, L"connect", L"connect to a game");
	
	return true;
}

bool MenuEventReceiver::HOST_GAME()
{
	Context.game_->startGame(true, ""); // call without ip, since we want to host 
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->init_ingame_menu();
	menuDone = true;
	return true;
}

//bool MenuEventReceiver::JOIN_WSDL() {
//	
//	Context.game_->init_map(Context.device, obstacles);
//	Context.game_->init_ingame_menu();
//	Context.game_->startGame();
//	menuDone = true;
//
//}

bool MenuEventReceiver::JOIN_GAME_SECOND()
{
	const wchar_t* t;
	t = box->getText();
	size_t   i;
	ch = (char *)malloc(30);
	wcstombs_s(&i, ch, (size_t)30, t, (size_t)30 );

	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	std::cout << ch;
	Context.game_->startGame(false, ch);
	Context.game_->init_map(Context.device, obstacles);
	Context.game_->init_ingame_menu();
	menuDone = true;

	return true;
}

bool MenuEventReceiver::END_GAME()
{
	/*Context.device->drop();
	cout << "this happened too";
	return true;*/

	Context.game_->init_map(Context.device, obstacles);
	Context.game_->localPlayer->setPlayer1(true);
	Context.game_->opposingPlayer->setPlayer1(false);
	Context.game_->localPlayer->initUnits();
	Context.game_->opposingPlayer->initUnits();
	Context.game_->init_ingame_menu();
	Context.game_->localPlayer->setActionsLeft();

	menuDone = true;

	return true;

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

	vector<BaseUnit*>::iterator it = getAllUnits()->begin();
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

	hoveredUnit = unitToBeShot = NULL;

	for (vector<BaseUnit*>::iterator it = highlightedEnemies->begin(); it != highlightedEnemies->end(); ++it)
		(*it)->highlightShoot(true);

	for(it; it != getAllUnits()->end() ; ++it)
	{
		// deselect the unit
		if (((*it)->player1 && Context.game_->localPlayer->getPlayer1()) || ((*it)->player1 == false && Context.game_->localPlayer->getPlayer1() == false))
			(*it)->highLightUnit(false);
		// if the positions of the node and the unit are the same
		// set this unit as the hovered unit and highlight it
		if((*it)->node->getPosition() == selectedPosition)
		{
			this->isHoveringUnit = true;

			if (this->isUnitSelected && (((*it)->player1 == false && Context.game_->localPlayer->getPlayer1()) || ((*it)->player1 == true && !Context.game_->localPlayer->getPlayer1())) && std::find(highlightedEnemies->begin(), highlightedEnemies->end(), (*it)) != highlightedEnemies->end()) {
				// Set unit to be shot
				unitToBeShot = (BaseUnit*)(*it);
				(*it)->highlightShoot(false);
			}
			else if (((*it)->player1 && Context.game_->localPlayer->getPlayer1()) || ((*it)->player1 == false && Context.game_->localPlayer->getPlayer1() == false)) {
				(*it)->highLightUnit(true);
				hoveredUnit = (BaseUnit*)(*it);
			}

			return;
		}
	}

	this->isHoveringUnit = false;

	if (hoveredGridNode != NULL) {
		hoveredGridNode->highlighted = false;
		hoveredGridNode->changeTexture();
	}

	hoveredGridNode = NULL;

	for (vector<GridNode*>::iterator it = highlightedNodes->begin(); it != highlightedNodes->end(); ++it) {

		if((*it)->node->getPosition() == selectedPosition)
		{
			hoveredGridNode = (GridNode*)(*it);
			hoveredGridNode->highlighted = true;
			hoveredGridNode->changeTexture();
			return;
		}

	}
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
	if(keyCode == irr::KEY_KEY_W || keyCode == irr::KEY_UP)
	{
		moveDirection = BaseUnit::FORWARD;
	}
	else if(keyCode == irr::KEY_KEY_S || keyCode == irr::KEY_DOWN)
	{
		moveDirection = BaseUnit::BACK;
	}
	else if(keyCode == irr::KEY_KEY_A || keyCode == irr::KEY_LEFT)
	{
		moveDirection = BaseUnit::LEFT;
	}
	else if(keyCode == irr::KEY_KEY_D || keyCode == irr::KEY_RIGHT)
	{
		moveDirection = BaseUnit::RIGHT;
	}
	// deselect the unit
	this->isUnitSelected = false;
	Context.game_->m->setUnitText("Click on a unit to see his stats");
	Context.game_->m->setActionText("Actions left = " + std::string(std::to_string(static_cast<long double>(Context.game_->localPlayer->getActionsLeft()))));
}

bool MenuEventReceiver::menu1()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
bool MenuEventReceiver::menu2()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
bool MenuEventReceiver::menu3()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
bool MenuEventReceiver::menu4()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
bool MenuEventReceiver::menu5()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
bool MenuEventReceiver::menu6()
{
	IGUIEnvironment* guienv = Context.device->getGUIEnvironment();
	guienv->clear();
	return true;
}
