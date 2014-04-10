#include "mapterrain.h"
#include <iostream>
using namespace std;
mapterrain::mapterrain(void)
{
}

mapterrain::mapterrain(IrrlichtDevice *device, ISceneManager *smgr)
{
		//code to load a map  ../media/map.irrmesh
	IAnimatedMesh* mesh = smgr->getMesh("../media/map.irrmesh");
	if (!mesh)
	{
		device->drop();
		
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	//node->setScale(core::vector3df(20,20,20));
	
	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "map mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;
}
mapterrain::~mapterrain(void)
{
}
