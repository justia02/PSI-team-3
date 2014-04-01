#pragma once
#include <vector3d.h>
#include <irrlicht.h>
#include <iostream>
#include <string>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

class BaseUnit
{
private:
	ILightSceneNode* selectIndication;
	IBillboardSceneNode* indicationBoard;
	void initUnitHighLight();
public:
	enum direction{
		LEFT,
		RIGHT,
		BACK,
		FORWARD
	};

	int id;

	int maxDistance;
	int health;
	int damage;
	irr::core::vector3d<float> position;
	
	//model variables
    ISceneManager* sceneManager;
    irr::io::path md2Path;
    irr::io::path bmpPath;
    IrrlichtDevice* device;
    IAnimatedMeshSceneNode* node;
    IVideoDriver* driver;


	bool selected;
	bool player1;

	BaseUnit(vector3d<float> position, bool player, IrrlichtDevice* dev);
	BaseUnit();
	~BaseUnit(void);

	void Move(direction moveDirection, float distance);
	void SelectUnit();
	
	bool createMesh();
    void setShininess(float value);

	void highLightUnit(bool highLight);
	// there needs to be some sort of target class to be made idk how yet
	//void ShootTarget(target);
};

