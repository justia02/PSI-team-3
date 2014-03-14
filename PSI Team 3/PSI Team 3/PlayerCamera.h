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
	PlayerCamera(void);
	~PlayerCamera(void);

	void setCameraPos(IrrlichtDevice *device, vector3d<float> pos);
};

