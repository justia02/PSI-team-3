#pragma once
#include <vector3d.h>
#include <irrlicht.h>
#include <iostream>
#include <string>
#include <vector>
#include "Obstacle.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

#define BALANCED_UNIT 0
#define MOVER 1
#define SHOOTER 2
#define BASE 3

class BaseUnit {

private:	
	bool hasShot;
	bool hasMoved;

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
	int shootingRange;

	bool highlightedToBeShot;

	irr::core::vector3d<float> position;
	
	//model variables
    ISceneManager* sceneManager;
    irr::io::path meshPath;
    irr::io::path texturePath;
    irr::io::path texturePathHighlight;
    irr::io::path texturePathSelected;
	irr::io::path texturePathToBeShot;
    IrrlichtDevice* device;
    IAnimatedMeshSceneNode* node;
    IVideoDriver* driver;
	scene::IBillboardTextSceneNode* healthBar;

	bool selected;
	bool player1;

	int onBaseCounter;

	BaseUnit(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);
	BaseUnit();
	~BaseUnit(void);

	bool canMove(direction moveDirection, float distance, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles);
	void Move(direction moveDirection, float distance, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles, bool player1, std::vector<vector3df>* fields);
	virtual void SelectUnit();
	virtual void setPosition(vector3df position) = 0;
	
	virtual bool createMesh() = 0;
	bool getHasMoved() { return hasMoved;}
	bool getHasShot() { return hasShot;}
	void setHasMoved(bool moved) {hasMoved = moved;}
	void setHasShot(bool shot) {hasShot = shot;}
	void setShininess(float value);

	virtual void highLightUnit(bool highLight);
	void highlightShoot(bool highlight);
	// there needs to be some sort of target class to be made idk how yet
	//void ShootTarget(target);

	BaseUnit* canShoot(direction shootDirection, std::vector<BaseUnit*>* units, std::vector<Obstacle*>* obstacles);
	void attack(BaseUnit* opponent);
	void setHealth(int health) {
		this->health = health;
	};
	int getHealth() {
		return health;
	};
	void remove();
	void updateHealthBar();

	bool canShoot(vector3df enemyPosition);

private:
	ILightSceneNode* selectIndication;
	IBillboardSceneNode* indicationBoard;
	direction revertDirection(direction direction);	

};

