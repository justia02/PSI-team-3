#include "BalancedUnit.h"

BalancedUnit::BalancedUnit(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 25;
	maxDistance = 2;
	shootingRange = 2;

	createMesh();

}

bool BalancedUnit::createMesh() {

	meshPath = "../media/girl.irrmesh";

	IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh){
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
        float scale = 0.02;
		node->setRotation(vector3df(0, 0, 0));
		vector3d<float> temp = vector3d<float>((0.5)/2, 0 , (0.5)/2);
		position += temp;
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, driver->getTexture(texturePath));
        node->setPosition(position);
        node->setScale(core::vector3df(scale,scale,scale));
		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0.5f, 2.5f, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}