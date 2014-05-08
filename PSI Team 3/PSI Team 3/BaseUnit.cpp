#include "BaseUnit.h"

// there needs to be a player given in the constructor as parameter soo the player1 boolean can be set for the unit 
// this way we can keep track of which unit is from which player.

#define HALF_SIZE 4

BaseUnit::BaseUnit(irr::core::vector3d<float> pos, bool player1, IrrlichtDevice* dev, int id)
{
	this->id = id;
	position = pos;
	health = 100;
	damage = 25;
	maxDistance = 2;
	shootingRange = 2;

	selected = false;
	hasMoved = false;
	hasShot = false;

    device = dev;
    sceneManager = device->getSceneManager();
    driver = device->getVideoDriver();
	this->player1 = player1;

    meshPath = "../media/testunit.irrmesh";
	if(player1){
		texturePath = "../media/Unit_P1.jpg";
		texturePathHighlight = "../media/Unit_P1_Highlight.jpg";
		texturePathSelected = "../media/Unit_P1_Selected.jpg";
	}else{
		texturePath = "../media/Unit_P2.jpg";
		texturePathHighlight = "../media/Unit_P2_Highlight.jpg";
		texturePathSelected = "../media/Unit_P2_Selected.jpg";
	}

	createMesh();

	//this->initUnitHighLight();
}

void BaseUnit::updateHealthBar() {

	std::string healthText = std::string(std::to_string(static_cast<long long>(getHealth())) + "%");
	healthBar->setText(std::wstring(healthText.begin(), healthText.end()).c_str());
	healthBar->setColor(irr::video::SColor(0, 0, 0, 0));

}


BaseUnit::~BaseUnit(void)
{
}

BaseUnit::direction BaseUnit::revertDirection(direction direction) {

	if (direction == LEFT)
		direction = RIGHT;
	else if (direction == RIGHT)
		direction = LEFT;
	else if (direction == BACK)
		direction = FORWARD;
	else if (direction == FORWARD)
		direction = BACK;

	return direction;

}

bool BaseUnit::canMove(direction moveDirection, float distance, std::vector<BaseUnit*>* units) {

	// Check whether selected unit can move in a given direction or not	
	switch(moveDirection) {
		case LEFT: {
			// Check if that would not violate board boundaries
			if (position.X < (-HALF_SIZE + distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (irr::core::vector3d<float>(position.X - distance, position.Y, position.Z) == (*it)->position)
					return false;
			break;
		}
		case RIGHT: {
			// Check if that would not violate board boundaries
			if (position.X > (HALF_SIZE - distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (irr::core::vector3d<float>(position.X + distance, position.Y, position.Z) == (*it)->position)
					return false;
			break;
		}
		case BACK: {
			// Check if that would not violate board boundaries
			if (position.Z < (-HALF_SIZE + distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (irr::core::vector3d<float>(position.X, position.Y, position.Z - distance) == (*it)->position)
					return false;
			break;
		}
		case FORWARD: {
			// Check if that would not violate board boundaries
			if (position.Z > (HALF_SIZE - distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (irr::core::vector3d<float>(position.X, position.Y, position.Z + distance) == (*it)->position)
					return false;
			break;
		}

	}

	return true;

}

void BaseUnit::shoot(direction shootDirection, std::vector<BaseUnit*>* units) {

	if (!player1)
		shootDirection = revertDirection(shootDirection);

	switch(shootDirection){
			case LEFT:{
				
				break;
				}
			case RIGHT:{
				
				break;
				}
			case BACK:{
				
				break;
				}
			case FORWARD:{
				
				break;
				}
		}

	float minZ = 10; // veeery biiiig at the beginning
	float dist;
	int opponnentHealth;

	for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
		if ((*it)->position.X == position.X) {
			if (fabs((*it)->position.Z - position.Z) < minZ) {
				minZ = (*it)->position.Z;
				opponnentHealth = (*it)->getHealth();
				dist = fabs((*it)->position.Z - position.Z);
			}
		}
	}

	if (minZ == 10) { // No units in the way
		std::cout << "Nothing to shoot!" << std::endl;
		return;
	}

	std::cout << "My Z-coord is: " << position.Z << std::endl;
	std::cout << "I'm going to attack the unit with Z-coord: " << minZ << std::endl;

	std::cout << "My HP before attack: " << health << std::endl;
	std::cout << "Opponent's unit HP before attack: " << opponnentHealth << std::endl;

	for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {

		if ((*it)->position.X == position.X && (*it)->position.Z == minZ) {
			// Attack! :D
			// Update health
			(*it)->setHealth((*it)->getHealth() - damage);
			// Update health bar
			(*it)->updateHealthBar();
			if (dist <= 1) { // Counter attack
				// Update health
				setHealth(getHealth() - (damage/2));
				// Update health bar
				updateHealthBar();
			}
			std::cout << "My HP after attack: " << health << std::endl;
			std::cout << "Opponent's unit HP after attack: " << (*it)->getHealth() << std::endl;

			// Remove the units from the map once they are dead
			if ((*it)->getHealth() <= 0) {
				(*it)->setHealth(0);
				(*it)->remove();
			}
			if (getHealth() <= 0) {
				setHealth(0);
				remove();
			}
			break;
		}	
	}
	setHasShot(true);
}

void BaseUnit::remove() {

	position.X = 100;
	position.Z = 100;

	node->setPosition(position);

}

void BaseUnit::Move(direction moveDirection, float distance, std::vector<BaseUnit*>* units, bool player1) {

	if(distance > maxDistance)
		distance = maxDistance;
	
	// Revert move direction if we're player2
	if (!player1)
		moveDirection = revertDirection(moveDirection);

	if (canMove(moveDirection, distance, units)) {
		switch(moveDirection){
			case LEFT:{
				position.X -= distance;
				break;
				}
			case RIGHT:{
				position.X += distance;
				break;
				}
			case BACK:{
				position.Z -= distance;
				break;
				}
			case FORWARD:{
				position.Z += distance;
				break;
				}
		}
	}
	
	setHasMoved(true);
	node->setPosition(position);
	cout << "the unit: " << "ID: " << id << "Position: " << position.X << ", " << position.Y << ", " << position.Z;

}

void BaseUnit::SelectUnit(){
	selected = !selected;
	if(selected)
		node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
	else if(!selected)
		node->setMaterialTexture(0, driver->getTexture(texturePath));

}

void BaseUnit::setShininess(float value){
    node->getMaterial(0).Shininess = value;
}

bool BaseUnit::createMesh(){
    if(device == NULL){
        cout << "The device is not set in MeshViewer->createMesh";
        return false;
    }
    
    if(sceneManager == NULL){
        cout << "The Scene manager is not set in MeshViewer->createMesh";
        return false;
    }
    
    if(driver == NULL){
        cout << "The driver is not set in MeshViewer->createMesh";
        return false;
    }
    IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh){
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
        float scale = 0.5;
		vector3d<float> temp = vector3d<float>((1-scale)/2, 0 , (1-scale)/2);
		position += temp;
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture(texturePath) );
        node->setPosition(position);
        node->setScale(core::vector3df(scale,scale,scale));
		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.6f, 0.6f), core::vector3df(0.5f, 2.5f, 0));
		healthBar->setColor(irr::video::SColor(0, 0, 0, 0));
    }

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "unit mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;
    return true;
}

void BaseUnit::highLightUnit(bool highLight)
{
	//selectIndication = NULL;
	if(highLight)
	{
		//Move(FORWARD, 2);
		if(player1)
			node->setMaterialTexture(0, driver->getTexture(texturePathHighlight));
		else
			node->setMaterialTexture(0, driver->getTexture(texturePathHighlight));
		//selectIndication->setVisible(true);
		//indicationBoard->setVisible(true);
	}
	else
	{
		if(selected)
			node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
		else if(!selected)
		{
			node->setMaterialTexture(0, driver->getTexture(texturePath));
			//selectIndication->setVisible(false);
			//indicationBoard->setVisible(false);
		}
	}
}

//void BaseUnit::initUnitHighLight()
//{
//	vector3d<f32> newPosition = this->position + vector3d<f32>(0,0.5,0);
//	selectIndication =
//		sceneManager->addLightSceneNode(0, position,
//		video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 800.0f);
//	selectIndication->setDebugDataVisible ( scene::EDS_BBOX );
//
//	selectIndication->setVisible(false);
//
//	// attach billboard to the light
//	indicationBoard =
//		sceneManager->addBillboardSceneNode(selectIndication, core::dimension2d<f32>(1, 1));
//
//	indicationBoard->setMaterialFlag(video::EMF_LIGHTING, false);
//	indicationBoard->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
//	indicationBoard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
//	indicationBoard->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/particlegreen.jpg"));
//
//	indicationBoard->setVisible(false);
//}
