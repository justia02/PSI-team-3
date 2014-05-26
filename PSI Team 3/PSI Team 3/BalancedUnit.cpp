#include "BalancedUnit.h"

BalancedUnit::BalancedUnit(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 25;
	maxDistance = 2;
	shootingRange = 2;

	texturePath = "../media/ant.jpg";
	texturePathHighlight = "../media/ant_highlighted.jpg";
	texturePathSelected = "../media/ant_selected.jpg";

	createMesh();

}

bool BalancedUnit::createMesh() {

	meshPath = "../media/ant.irrmesh";

	IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh){
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    
    if (node)
    {
        float scale = 9;
		if (!player1)
			node->setRotation(vector3df(0, 180, 0));

        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        setPosition(position);
        node->setScale(core::vector3df(scale, scale, scale));
		node->setMaterialTexture(0, driver->getTexture(texturePath));

		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0, 0.1, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}

void BalancedUnit::setPosition(vector3df position) {

	(*this).position = position;

	vector3df temp;

	if (player1)
		temp = vector3df(0.5, 0, 0.6);					
	else
		temp = vector3df(0.5, 0, 0.5);

	node->setPosition(position + temp);

}