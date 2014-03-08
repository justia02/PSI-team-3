#pragma once
#include <vector3d.h>

namespace core = irr::core;

class BaseUnit
{
public:
	int maxDistance;
	int health;
	int damage;
	core::vector3d<float> position;

	bool selected;
	bool player1;

	BaseUnit(core::vector3d<float> position);
	~BaseUnit(void);

	void MoveLeft(float distance);
	void MoveRight(float distance);
	void MoveBack(float distance);
	void MoveForward(float distance);

	void SelectUnit();


	// there needs to be some sort of target class to be made idk how yet
	//void ShootTarget(target);
};

