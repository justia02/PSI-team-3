#include "PlayerCamera.h"
#include <iostream>
using namespace std;

PlayerCamera::PlayerCamera(IrrlichtDevice *dev)
{
	device = dev;
}


PlayerCamera::~PlayerCamera(void)
{
}

void PlayerCamera::setCameraPos(vector3d<float> pos, bool player1){
	ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
	//vector3df cameraPos = camera->getAbsolutePosition();

	float x = pos.X;// - sin( -90.0f * PI / 180.0f ) * 0.0f;
    float y = pos.Y - sin( -90.0f * PI / 180.0f ) * 8.0f;
	float z;
	if(player1)
		z = pos.Z - sin( 90.0f * PI / 180.0f ) * 8.0f;
	else
		z = pos.Z + sin( 90.0f * PI / 180.0f ) * 8.0f;

	cout<< "xpos " << x << "\nypos " << y << "\nzpos " << z << "\n";
	camera->setPosition(vector3df( x, y, z));
	camera->setTarget(vector3df(pos.X, pos.Y, pos.Z));
}