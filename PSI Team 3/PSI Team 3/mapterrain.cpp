#include "mapterrain.h"
#include "Obstacle.h"
#include <iostream>


using namespace std;
mapterrain::mapterrain(void)
{
}

mapterrain::mapterrain(IrrlichtDevice *device, ISceneManager *smgr, std::vector<Obstacle*>* obstacles)
{
	loadBases(smgr);

		//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "map mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;

	// Add obstacles:
	// Pyramid
	obstacles->at(0)->createMesh();
	obstacles->at(0)->setPosition(vector3d<float>(1.25, 0.45, -0.75));
	// Spider
	obstacles->at(1)->createMesh();
	obstacles->at(1)->setPosition(vector3d<float>(-2.75, 0.45, -0.75));
	// Cat
	obstacles->at(2)->createMesh();
	obstacles->at(2)->setPosition(vector3d<float>(2.25, 0.7, 1.25));

}
mapterrain::~mapterrain(void)
{
}

//this function loads the basses on the map
void mapterrain::loadBases(ISceneManager *smgr)
{
	//load the base models here
	IAnimatedMesh* base1 = smgr->getMesh("../media/base1.irrmesh");
	IAnimatedMesh* base2 = smgr->getMesh("../media/base2.irrmesh");

	//put the bases in the scene
	IAnimatedMeshSceneNode* node1 = smgr->addAnimatedMeshSceneNode( base1 );
	IAnimatedMeshSceneNode* node2 = smgr->addAnimatedMeshSceneNode( base2 );


	//these are the positions for the basses
	irr::core::vector3df pos1 = vector3d<float>(0, 0, -3.55);
	irr::core::vector3df pos2 = vector3d<float>(0, 0, 3.45);

	//reposition the basses to their place
	node1->setPosition(pos1);
	node2->setPosition(pos2);

}