#pragma once
#include <vector3d.h>

class BaseUnit
{
public:
	int maxDistance;
	int health;
	int damage;
	irr::core::vector3d<float> position;

	bool selected;
	bool player1;

	BaseUnit(irr::core::vector3d<float> position);
	BaseUnit();
	~BaseUnit(void);

	void MoveLeft(float distance);
	void MoveRight(float distance);
	void MoveBack(float distance);
	void MoveForward(float distance);

	void SelectUnit();


	// there needs to be some sort of target class to be made idk how yet
	//void ShootTarget(target);
};

