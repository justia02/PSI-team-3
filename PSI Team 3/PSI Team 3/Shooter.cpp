#include "Shooter.h"

Shooter::Shooter(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 30;
	maxDistance = 1;
	shootingRange = 3;

	texturePath = "../media/ironMan.png";
	texturePathHighlight = "../media/ironMan_highlighted.png";
	texturePathSelected = "../media/ironMan_selected.png";
	if (player1) {
		texturePath = "../media/ironMan_p1.png";
		texturePathHighlight = "../media/ironMan_highlighted_p1.png";
		texturePathSelected = "../media/ironMan_selected_p1.png";
	}
	else {
		texturePath = "../media/ironMan.png";
		texturePathHighlight = "../media/ironMan_highlighted.png";
		texturePathSelected = "../media/ironMan_selected.png";
	}
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
		
		if (player1 == false)
			node->setRotation(vector3df(0, 180, 0));

        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        setPosition(position);
        node->setScale(core::vector3df(scale, scale/2, scale));
		node->setMaterialTexture(0, driver->getTexture(texturePath));

		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0, 2.5f, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}

void Shooter::setPosition(vector3df position) {

	(*this).position = position;

	vector3df temp;
		
	if (player1 == true)
		temp = vector3df(0.4, 0, 0.3);
	else		
		temp = vector3df(0.6, 0, 0.5);

	node->setPosition(position + temp);

}