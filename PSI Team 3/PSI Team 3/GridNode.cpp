#include "GridNode.h"

GridNode::GridNode(ISceneManager* smgr, IAnimatedMesh* mesh, vector3df position) {

	texturePath = "../media/UnitMoveLocation.jpg";
	textureHighLightPath = "../media/UnitMove.jpg";
	this->smgr = smgr;

	node = smgr->addAnimatedMeshSceneNode(mesh);
    
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture(texturePath));
		node->setPosition(position + vector3df(0.4, 0, 0));
		node->setScale(vector3df(0.3, 0.3, 0.3));
	}

	this->position = position;

	highlighted = false;

}

void GridNode::changeTexture() {

	if (node) {
		if (highlighted)
			node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture(textureHighLightPath));
		else
			node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture(texturePath));
	}

}