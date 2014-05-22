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
				// node->setScale(vector3df(0.004, 0.004, 0.004));
				// node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/black.jpg"));
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

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "Obstacle mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;

}

void Obstacle::setPosition(vector3df position) {

	this->position = position;

	switch(obstacleType) {
		case type::PYRAMID: {
			node->setPosition(vector3df(position.X + 0.20, position.Y, position.Z));
			break;
		}
		case type::SPIDER: {
			node->setPosition(vector3df(position.X + 0.2, position.Y, position.Z));
			break;
		}
		case type::CAT: {
			node->setPosition(vector3df(position.X, position.Y, position.Z - 0.6));
			break;
		}
	}
	
}