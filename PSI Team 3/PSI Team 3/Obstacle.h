#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

enum type {
	PYRAMID,
	TREE,
	SPIDER
};

class Obstacle {

	private:
		type obstacleType;
		IrrlichtDevice* device;
		IAnimatedMesh* mesh;
		IAnimatedMeshSceneNode* node;		

	public:
		vector3d<float> position;
		Obstacle(type obstacleType, IrrlichtDevice* device);
		void setPosition(vector3d<float> position);
		void createMesh();

};

#endif