#include "Mover.h"

Mover::Mover(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id) : BaseUnit(position, player1, dev, id) {

	damage = 30;
	maxDistance = 3;
	shootingRange = 1;

	normalMesh = selectedMesh = highlightedMesh = NULL;

	createMesh();

};

void Mover::SelectUnit() {

	selected = !selected;

	if(selected)
		node->setMesh(sceneManager->getMesh("../media/van_selected.irrmesh"));
	else
		node->setMesh(sceneManager->getMesh("../media/van.irrmesh"));

}

void Mover::highLightUnit(bool highLight) {

	if(highLight) {
		if (highlightedMesh == NULL)
			highlightedMesh = sceneManager->getMesh("../media/van_highlighted.irrmesh");
		node->setMesh(highlightedMesh);
	}
	else
	{
		if(selected) {
			if (selectedMesh == NULL)
				selectedMesh = sceneManager->getMesh("../media/van_selected.irrmesh");
			node->setMesh(selectedMesh);
		}
		else
			node->setMesh(normalMesh);
	}

}

bool Mover::createMesh() {

	normalMesh = sceneManager->getMesh("../media/van.irrmesh");
    
    if (!normalMesh) {
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }

    node = sceneManager->addAnimatedMeshSceneNode(normalMesh);
    
    if (node)
    {
		float scale = 0.004;
		vector3d<float> temp;
		if (player1) {
			temp = vector3df(0.5, 0, 0.5);
			node->setRotation(vector3df(0, 180, 0));
		}
		else {
			temp = vector3df(0.5, 0, 0.5);
		}
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setPosition(position + temp);
        node->setScale(core::vector3df(scale, scale, scale));
		// Add health bar
		healthBar = sceneManager->addBillboardTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"100%", node, core::dimension2d<f32>(0.5f, 0.5f), core::vector3df(0, 200, 0));
		healthBar->setColor(irr::video::SColor(0, 255, 0, 0));
    }

    return true;

}