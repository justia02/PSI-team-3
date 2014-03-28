#pragma once
#include <ICameraSceneNode.h>
#include <vector3d.h>
#include <IrrlichtDevice.h>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class PlayerCamera
{
public:
	PlayerCamera(IrrlichtDevice *dev);
	~PlayerCamera(void);
	
	IrrlichtDevice *device;
	void setCameraPos(vector3d<float> pos, bool player1);
};

