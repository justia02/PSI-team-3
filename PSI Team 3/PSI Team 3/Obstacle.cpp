#include "Obstacle.h"

Obstacle::Obstacle(type oType, IrrlichtDevice* device) {
	obstacleType = oType;
	this->device = device;
}

void Obstacle::createMesh() {

	switch(obstacleType) {
		case type::PYRAMID: {
			mesh = device->getSceneManager()->getMesh("../media/pyramid.irrmesh");
			break;
		}
		case type::SPIDER: {
			mesh = device->getSceneManager()->getMesh("../media/spider.irrmesh");
			break;
		}
		case type::CAT: {
			mesh = device->getSceneManager()->getMesh("../media/cat.irrmesh");
			break;
		}
		default: { // None of above
			cout << "Invalid obstacle type specified!" << endl;
			return;
		}
	}
    
    if (!mesh) {
        cout << "The mesh could not be created in MashViewer->createMesh";
        return;
    }

    node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
		switch(obstacleType) {
			case type::PYRAMID: {
				node->setScale(vector3df(0.33, 0.3, 0.33));
				node->setRotation(vector3df(0, 45, 0));
				break;
			}
			case type::SPIDER: {
				node->setScale(vector3df(9, 9, 9));
				node->setRotation(vector3df(0, 135, 0));				
				break;
			}
			case type::CAT: {
				node->setScale(vector3df(1, 1, 1));
				node->setRotation(vector3df(0, -135, 0));
				break;
			}
		}
    }

}

void Obstacle::setPosition(vector3df position, bool player1) {

	this->position = position;

	switch(obstacleType) {
		case type::PYRAMID: {
			if (player1)
				node->setPosition(vector3df(position.X + 0.45, position.Y, position.Z + 0.3));
			else
				node->setPosition(vector3df(position.X + 0.45, position.Y, position.Z + 0.7));
			break;
		}
		case type::SPIDER: {
			if (player1)
				node->setPosition(vector3df(position.X + 0.5, position.Y, position.Z + 0.35));
			else
				node->setPosition(vector3df(position.X + 0.5, position.Y, position.Z + 0.75));
			break;
		}
		case type::CAT: {
			if (player1)
				node->setPosition(vector3df(position.X + 0.25, position.Y, position.Z - 0.2));
			else
				node->setPosition(vector3df(position.X + 0.25, position.Y, position.Z + 1.0));
			break;
		}
	}
	
}