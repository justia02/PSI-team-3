#include "mapterrain.h"
#include "Obstacle.h"
#include <iostream>


using namespace std;
mapterrain::mapterrain(void)
{
}

mapterrain::mapterrain(IrrlichtDevice *device, ISceneManager *smgr, std::vector<Obstacle*>* obstacles, bool player1)
{
	loadBases(smgr);

		//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	// Add obstacles:
	// Pyramid
	obstacles->at(0)->createMesh();
	obstacles->at(0)->setPosition(vector3d<float>(1, 0.45, -1), player1);
	// Spider
	obstacles->at(1)->createMesh();
	obstacles->at(1)->setPosition(vector3d<float>(-3, 0.45, -1), player1);
	// Cat
	obstacles->at(2)->createMesh();
	obstacles->at(2)->setPosition(vector3d<float>(2, 0.7, 1), player1);

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