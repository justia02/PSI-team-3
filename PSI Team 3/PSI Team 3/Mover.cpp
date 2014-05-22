#include "Mover.h"

Mover::Mover(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 30;
	maxDistance = 3;
	shootingRange = 1;

	createMesh();

};


bool Mover::createMesh() {

	meshPath = "../media/van.irrmesh";

    IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh) {
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
		float scale = 0.005;
		vector3d<float> temp = vector3d<float>((0.5)/2, 0.1, (0.5)/2);
		position += temp;
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        // node->setMaterialTexture(0, driver->getTexture(texturePath));
        node->setPosition(position);
        node->setScale(core::vector3df(scale, scale, scale));
		if (player1 == true)
			node->setRotation(vector3df(0, 180, 0));
		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0.5f, 30, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}