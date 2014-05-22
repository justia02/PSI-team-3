#include "Shooter.h"

Shooter::Shooter(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 30;
	maxDistance = 1;
	shootingRange = 3;

	createMesh();

};

bool Shooter::createMesh() {

	meshPath = "../media/ironMan.irrmesh";

    IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh) {
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
		float scale = 1;
		vector3d<float> temp = vector3d<float>((0.5)/2, 0, (0.5)/2);
		position += temp;
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setPosition(position);
        node->setScale(core::vector3df(scale, scale/2, scale));
		if (player1 == false)
			node->setRotation(vector3df(0, 180, 0));
		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0.5f, 2.5f, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}