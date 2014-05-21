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
		case type::TREE: {
			mesh = device->getSceneManager()->getMesh("../media/tree.irrmesh");
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
		node->setScale(vector3df(0.3, 0.3, 0.3));
		if (obstacleType == type::SPIDER) {
			node->setScale(vector3df(0.004, 0.004, 0.004));
			node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/black.jpg"));
		}
		// node->setPosition(vector3df(position.X + 0.2, position.Y, position.Z));
		node->setRotation(vector3df(0, 45, 0));
		// node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/wall_light.jpg"));
    }

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "Obstacle mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;

}

void Obstacle::setPosition(vector3df position) {

	this->position = position;
	node->setPosition(vector3df(position.X + 0.3, position.Y - 0.3, position.Z));

}