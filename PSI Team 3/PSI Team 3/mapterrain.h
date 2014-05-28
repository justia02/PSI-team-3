#pragma once
#include <irrlicht.h>
#include "Player.h"

using namespace irr;
using namespace scene;

class mapterrain
{
public:
	mapterrain(void);

	mapterrain(IrrlichtDevice *device, ISceneManager* smgr, std::vector<Obstacle*>* obstacles, bool player1);

	~mapterrain(void);

	void loadBases(ISceneManager* smgr);

	void createSkybox(IrrlichtDevice *device, ISceneManager *smgr);
	IVideoDriver* driver;

	ITexture* top;
	ITexture* bottom;
	ITexture* front;
	ITexture* back;
	ITexture* left;
	ITexture* right;
};

