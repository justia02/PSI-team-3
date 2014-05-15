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
		case type::BIG_PYRAMID: {
			mesh = device->getSceneManager()->getMesh("../media/bigPyramid.irrmesh");
			break;
		}
		case type::TREE: {
			// mesh = sceneManager->getMesh("../media/pyramid3.irrmesh");
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
		node->setScale(vector3df(0.12, 0.12, 0.12));
		node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/wall_light.jpg"));
    }

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "Obstacle mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;

}

void Obstacle::setPosition(vector3df position) {

	node->setPosition(this->position = position);

}