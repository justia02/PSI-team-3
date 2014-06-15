#include "menu.h"

/**
  * Implementation of menu
  */

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv, int horizontal, int vertical) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;

	
	
	//driver->draw2DImage("../media/4.jpg");

	width = horizontal;
	height = vertical;

	// initalizes the in game info menu
	init();
}

menu::~menu(void)
{
}


/**
 * inializes the graphical menu
 */
void menu::init(void)
{
	//put font for the texts
	Font = guienv->getFont("../media/fonts/candara14.bmp");

	// Make the texts that will always be available
	//playerText = guienv->addStaticText(L"", rect<s32>((width / 7), (height/ 30) * 1, (width / 3) , (height/ 30) * 2), false, false, NULL, 1, true);
	//playerText->setOverrideFont(Font);

	turnText = guienv->addStaticText(L"It is your turn", rect<s32>((width / 7), (height/ 30) * 1, (width / 3) , (height/ 30) * 2), false, false, NULL, 1, true);
	turnText->setOverrideFont(Font);

	actionText = guienv->addStaticText(L"Actions Left = ", rect<s32>((width / 7), (height/ 30) * 2, (width / 3), (height/ 30) * 4), false, false, NULL, 2, true);
	actionText->setOverrideFont(Font);

	action0 = driver->getTexture("../media/0.png");
	action1 = driver->getTexture("../media/1.png");
	action2 = driver->getTexture("../media/2.png");
	action3 = driver->getTexture("../media/3.png");
	action4 = driver->getTexture("../media/4.png");
	action_image = guienv->addImage(action4, position2d<int>((width / 8) * 2, (height/ 30) * 1));
	

	//driver->draw2DImage(actions,  rect<s32>(0, 0, 300, 300), rect<s32>(0, 0, 300, 300));

	unitText = guienv->addStaticText(L"Click on a unit to see his stats", rect<s32>((width / 3) , (height/ 30) * 1, (width/ 3) * 2, (height/ 30) * 4), false, false, NULL, 4, true);
	unitText->setOverrideFont(Font);


	waitText = guienv->addStaticText(L" ", rect<s32>((width / 3) , (height/ 30) * 27, (width / 3) * 2, (height / 30) * 29), false, false, NULL, 6, true);
	waitText->setOverrideFont(Font);
}

void menu::setPlayerText(std::string playertext)
{
	playerText->setText(std::wstring(playertext.begin(), playertext.end()).c_str());
}

void menu::setActionText(std::string actiontext)
{
	actionText->setText(std::wstring(actiontext.begin(), actiontext.end()).c_str());
}

void menu::setTurnText(std::string turntext)
{
	turnText->setText(std::wstring(turntext.begin(), turntext.end()).c_str());
}

void menu::setUnitText(std::string unittext)
{
	unitText->setText(std::wstring(unittext.begin(), unittext.end()).c_str());
}


void menu::setWaitText(bool wait)
{
	if(wait)
	{
		waitText->setText(L"Please Wait!!!");
		waitText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	}
	else{
		waitText->setText(L" ");
	}
}

void menu::setActionImage(int number)
{
	cout << number << endl;
	if(number == 0){
			action_image->setImage(action0);
			cout << "This happened 0";
	}

	else if(number == 1){
			action_image->setImage(action1);
			cout << "This happened 1";
	}

	else if(number == 2){
			action_image->setImage(action2);
			cout << "This happened 2";
	}

	else if(number == 3){	
			action_image->setImage(action3);
			cout << "This happened 3";
	}

	else if(number == 4){	
			action_image->setImage(action4);
			cout << "This happened 4";
	}
	
	
}
