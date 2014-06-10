#include "BaseUnit.h"

// there needs to be a player given in the constructor as parameter soo the player1 boolean can be set for the unit 
// this way we can keep track of which unit is from which player.

#define HALF_SIZE 4

BaseUnit::BaseUnit(irr::core::vector3d<float> pos, bool player1, IrrlichtDevice* dev, int id)
{
	this->id = id;
	position = pos;
	health = 100;

	selected = false;
	hasMoved = false;
	hasShot = false;

    device = dev;
    sceneManager = device->getSceneManager();
    driver = device->getVideoDriver();

	if(device == NULL) {
        cout << "The device is not set in MeshViewer->createMesh";
        return;
    }
    
    if(sceneManager == NULL) {
        cout << "The Scene manager is not set in MeshViewer->createMesh";
        return;
    }
    
    if(driver == NULL) {
        cout << "The driver is not set in MeshViewer->createMesh";
        return;
    }

	this->player1 = player1;

}

void BaseUnit::updateHealthBar() {

	std::string healthText = std::string(std::to_string(static_cast<long long>(getHealth())) + "%");
	healthBar->setText(std::wstring(healthText.begin(), healthText.end()).c_str());
	healthBar->setColor(irr::video::SColor(0, 255, 0, 0));

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

bool BaseUnit::canMove(direction moveDirection, float distance, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles) {

	// Check whether selected unit can move in a given direction or not	
	switch(moveDirection) {
		case LEFT: {
			// Check if that would not violate board boundaries
			if (position.X < (-HALF_SIZE + distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if ((position.X - distance) == (*it)->position.X && position.Z == (*it)->position.Z)
					return false;
			// Check if an obstacle is not there
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it)
				if ((position.X - distance) == (*it)->position.X && position.Z == (*it)->position.Z)
					return false;
			break;
		}
		case RIGHT: {
			// Check if that would not violate board boundaries
			if (position.X >= (HALF_SIZE - distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if ((position.X + distance) == (*it)->position.X && position.Z == (*it)->position.Z)
					return false;
			// Check if an obstacle is not there
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it)
				if ((position.X + distance) == (*it)->position.X && position.Z == (*it)->position.Z)
					return false;
			break;
		}
		case BACK: {
			// Check if that would not violate board boundaries
			if (position.Z < (-HALF_SIZE + distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (position.X == (*it)->position.X && (position.Z - distance) == (*it)->position.Z)
					return false;
			// Check if an obstacle is not there
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it)
				if (position.X == (*it)->position.X && (position.Z - distance) == (*it)->position.Z)
					return false;
			break;
		}
		case FORWARD: {
			// Check if that would not violate board boundaries
			if (position.Z >= (HALF_SIZE - distance))
				return false;
			// Check if another unit isn't already there
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it)
				if (position.X == (*it)->position.X && (position.Z + distance) == (*it)->position.Z)
					return false;
			// Check if an obstacle is not there
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it)
				if (position.X == (*it)->position.X && (position.Z + distance) == (*it)->position.Z)
					return false;
			break;
		}

	}

	return true;

}

bool BaseUnit::canShoot(vector3df enemyPosition) {

	if (position.getDistanceFrom(enemyPosition) <= shootingRange)
		return true;

	return false;

}

BaseUnit* BaseUnit::canShoot(direction shootDirection, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles) {

	if (player1 == false)
		shootDirection = revertDirection(shootDirection);

	float minAttackDirectionCoordinate = 10; // Bigger than board size, will be used to find a minimum
	float minDistance = 10; // Bigger than board size, will be used to find a minimum
	float distanceFromTheOpponent = 0;
	
	switch(shootDirection) {
		case LEFT: {
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X < position.X) {
					if (fabs((*it)->position.X - position.X) < minDistance) {
						minDistance = fabs((*it)->position.X - position.X);
						minAttackDirectionCoordinate = (*it)->position.X;
						if (minDistance <= shootingRange)
							distanceFromTheOpponent = minDistance;
					}
				}
			}
			break;
		}
		case RIGHT: {
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X > position.X) {
					if (fabs((*it)->position.X - position.X) < minDistance) {
						minDistance = fabs((*it)->position.X - position.X);
						minAttackDirectionCoordinate = (*it)->position.X;
						if (minDistance <= shootingRange)
							distanceFromTheOpponent = minDistance;
					}
				}
			}
			break;
		}
		case BACK: {
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z < position.Z) {
					if (fabs((*it)->position.Z - position.Z) < minDistance) {
						minDistance = fabs((*it)->position.Z - position.Z);
						minAttackDirectionCoordinate = (*it)->position.Z;
						if (minDistance <= shootingRange)
							distanceFromTheOpponent = minDistance;
					}
				}
			}
			break;
		}
		case FORWARD: {
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z > position.Z) {
					if (fabs((*it)->position.Z - position.Z) < minDistance) {
						minDistance = fabs((*it)->position.Z - position.Z);
						minAttackDirectionCoordinate = (*it)->position.Z;
						if (minDistance <= shootingRange)
							distanceFromTheOpponent = minDistance;
					}
				}
			}
			break;
		}
	}

	if (distanceFromTheOpponent == 0) { // No enemy in the range
		return NULL;
	}
	if (minAttackDirectionCoordinate == 10) { // No enemy units in the way
		return NULL;
	}	

	switch(shootDirection) {
		case LEFT: {
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X < position.X && (*it)->position.X > minAttackDirectionCoordinate) {
					return NULL;
				}
			}
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X == minAttackDirectionCoordinate) {
					return (*it);
				}
			}
			break;
		}
		case RIGHT: {
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X > position.X && (*it)->position.X < minAttackDirectionCoordinate) {
					return NULL;
				}
			}
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.Z == position.Z && (*it)->position.X == minAttackDirectionCoordinate) {
					return (*it);
				}	
			}
			break;
		}
		case BACK: {
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z < position.Z && (*it)->position.Z > minAttackDirectionCoordinate) {
					return NULL;
				}
			}
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z == minAttackDirectionCoordinate) {
					return (*it);
				}	
			}
			break;
		}
		case FORWARD: {
			for(vector<Obstacle*>::iterator it = obstacles->begin(); it != obstacles->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z > position.Z && (*it)->position.Z < minAttackDirectionCoordinate) {
					return NULL;
				}
			}
			for(vector<BaseUnit*>::iterator it = units->begin(); it != units->end(); ++it) {
				if ((*it)->position.X == position.X && (*it)->position.Z == minAttackDirectionCoordinate) {
					return (*it);
				}	
			}
			break;
		}
	}

}

void BaseUnit::attack(BaseUnit* opponent) {

	// Update health
	opponent->setHealth(opponent->getHealth() - damage);
	// Update health bar
	opponent->updateHealthBar();

	float distance = position.getDistanceFrom(opponent->position);
	if (distance <= 1) { // Counter attack
		// Update health
		setHealth(getHealth() - (damage/2));
		// Update health bar
		updateHealthBar();
	}

	// Remove the units from the map once they are dead
	if (opponent->getHealth() <= 0) {
		opponent->setHealth(0);
		opponent->remove();
	}
	if (getHealth() <= 0) {
		setHealth(0);
		remove();
	}

	setHasShot(true);

}

void BaseUnit::remove() {

	position.X = 100;
	position.Z = 100;

	node->setPosition(position);

}

void BaseUnit::Move(direction moveDirection, float distance, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles, bool player1, std::vector<vector3df>* fields) {

	if(distance > maxDistance)
		distance = maxDistance;
	
	// Revert move direction if we're player2
	if (!player1)
		moveDirection = revertDirection(moveDirection);

	int stepCounter = 1;
	while (canMove(moveDirection, stepCounter, units, obstacles) == true) {
		stepCounter++;
	}

	if (--stepCounter < distance)
		distance = stepCounter;
	if (stepCounter == 0)
		return;
	
	switch(moveDirection){
		case LEFT:{
			position.X -= distance;
			setPosition(position);
			break;
			}
		case RIGHT:{
			position.X += distance;
			setPosition(position);
			break;
			}
		case BACK:{
			position.Z -= distance;
			setPosition(position);
			break;
			}
		case FORWARD:{
			position.Z += distance;
			setPosition(position);
			break;
			}
	}
	
	setHasMoved(true);
	cout << "the unit: " << "ID: " << id << "Position: " << position.X << ", " << position.Y << ", " << position.Z;

}

void BaseUnit::SelectUnit(){
	selected = !selected;
	if(selected)
		node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
	else
		node->setMaterialTexture(0, driver->getTexture(texturePath));
}

void BaseUnit::setShininess(float value){
    node->getMaterial(0).Shininess = value;
}

void BaseUnit::highLightUnit(bool highLight)
{
	if(highLight)
		node->setMaterialTexture(0, driver->getTexture(texturePathHighlight));
	else
	{
		if(selected)
			node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
		else
			node->setMaterialTexture(0, driver->getTexture(texturePath));
	}
}

void BaseUnit::highlightShoot(bool highlight) {

	if (highlight)
		node->setMaterialTexture(0, sceneManager->getVideoDriver()->getTexture("../media/ant_highlighted.jpg"));
	else
		node->setMaterialTexture(0, sceneManager->getVideoDriver()->getTexture("../media/ant_selected.jpg"));

	highlightedToBeShot = highlight;

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
