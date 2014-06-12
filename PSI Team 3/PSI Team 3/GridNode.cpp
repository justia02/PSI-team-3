#include "GridNode.h"

GridNode::GridNode(ISceneManager* smgr, IAnimatedMesh* mesh, vector3df position) {

	texturePath = "../media/UnitMoveLocation.jpg";
	textureHighLightPath = "../media/UnitMove.jpg";
	this->smgr = smgr;

	node = smgr->addAnimatedMeshSceneNode(mesh);
    
	if (node)
	{
		node->setPosition(position + vector3df(0.5, 0, 0.5));
		node->setScale(vector3df(0.8, 0.6, 0.8));
	}

	this->position = position;

	highlighted = false;

}

void GridNode::changeTexture() {

	if (node) {
		if (highlighted)
			node->setMesh(smgr->getMesh("../media/gridNodeHighlight.irrmesh"));
		else
			node->setMesh(smgr->getMesh("../media/gridNode.irrmesh"));
	}

}