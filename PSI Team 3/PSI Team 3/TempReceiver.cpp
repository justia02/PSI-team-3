#include "TempReceiver.h"

TempReceiver::TempReceiver(IrrlichtDevice *device, Player *player)
{
	this->device = device;
	this->unitList = player->getUnits();
	this->smgr = device->getSceneManager();
	this->camera = smgr->getActiveCamera();
	this->collManager = smgr->getSceneCollisionManager();
}


TempReceiver::~TempReceiver(void)
{
}

bool TempReceiver::OnEvent(const SEvent& event)
{
	
	// Remember the mouse state
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        switch(event.MouseInput.Event)
        {
        case EMIE_LMOUSE_PRESSED_DOWN:
			if(this->isHoveringUnit)
			{

				//moveUnit();
				selectedUnit->SelectUnit();
				//this->selectUnit(this->selectedUnit);
				
				//if(this->selectedUnit != NULL)
				//	this->selectedUnit->Move(BaseUnit::BACK,2);
			}
            break;
		case EMIE_MOUSE_MOVED:
			MouseOverUnit();
			break;
		}
	}
	return 0;
}

void TempReceiver::moveUnit()
{
	position2d<irr::s32> cursorPos;
	vector3df selectedPosition;
	vector<BaseUnit*>::iterator it = this->unitList->begin();

	cursorPos = device->getCursorControl()->getPosition();
	line = collManager->getRayFromScreenCoordinates(cursorPos, camera);
	selectedNode = collManager->getSceneNodeFromRayBB(line);
	selectedPosition = selectedNode->getAbsolutePosition();

	for(it ; it != unitList->end() ; ++it)
	{
		if((*it)->position == selectedPosition)
		{
			(*it)->Move(BaseUnit::FORWARD, 2);
		}
	}
}


void TempReceiver::MouseOverUnit()
{
	position2d<irr::s32> cursorPos;
	vector3df selectedPosition;
	vector<BaseUnit*>::iterator it = this->unitList->begin();

	cursorPos = device->getCursorControl()->getPosition();
	line = collManager->getRayFromScreenCoordinates(cursorPos, camera);
	selectedNode = collManager->getSceneNodeFromRayBB(line);
	selectedPosition = selectedNode->getAbsolutePosition();

	for(it ; it != unitList->end() ; ++it)
	{
		// deselect the unit
		(*it)->highLightUnit(false);
		//(*it)->selected = false;
		if((*it)->position == selectedPosition)
		{
			this->isHoveringUnit = true;
			//(*it)->SelectUnit();
			(*it)->highLightUnit(true);
			selectedUnit = (BaseUnit*)(*it);
			return;
			//it->Move(BaseUnit::FORWARD, 3);
			//cout << "We have a seleced unit!";
		}
	}
	this->isHoveringUnit = false;
}

void TempReceiver::selectUnit(BaseUnit *unit)
{
	vector3d<f32> newPosition = unit->position + vector3d<f32>(0,0.5,0);
	
	ILightSceneNode* light1 =
		smgr->addLightSceneNode(0, newPosition,
		video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 800.0f);
	light1->setDebugDataVisible ( scene::EDS_BBOX );

	// attach billboard to the light
	IBillboardSceneNode* bill =
		smgr->addBillboardSceneNode(light1, core::dimension2d<f32>(1, 1));

	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/particlegreen.jpg"));
}

