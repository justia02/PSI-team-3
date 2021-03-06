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
			mesh = device->getSceneManager()->getMesh("../media/pyramid.irrmesh");
			break;
		}
		case type::CAT: {
			mesh = device->getSceneManager()->getMesh("../media/pyramid.irrmesh");
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
				node->setScale(vector3df(0.33, 0.3, 0.33));
				node->setRotation(vector3df(0, 45, 0));			
				break;
			}
			case type::CAT: {
				node->setScale(vector3df(0.33, 0.3, 0.33));
				node->setRotation(vector3df(0, 45, 0));
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
				node->setPosition(vector3df(position.X + 0.4, position.Y, position.Z + 0.3));
			else
				node->setPosition(vector3df(position.X + 0.45, position.Y, position.Z + 0.7));
			break;
		}
		case type::SPIDER: {
			if (player1)
				node->setPosition(vector3df(position.X + 0.4, position.Y, position.Z));
			else
				node->setPosition(vector3df(position.X + 0.4, position.Y, position.Z + 0.9));
			break;
		}
		case type::CAT: {
			if (player1)
				node->setPosition(vector3df(position.X + 0.6, position.Y, position.Z + 0.3));
			else
				node->setPosition(vector3df(position.X + 0.55, position.Y, position.Z + 0.7));
			break;
		}
	}
	
}