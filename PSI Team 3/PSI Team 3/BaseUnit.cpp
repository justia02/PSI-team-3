#include "BaseUnit.h"

// there needs to be a player given in the constructor as parameter soo the player1 boolean can be set for the unit 
// this way we can keep track of which unit is from which player.
BaseUnit::BaseUnit(irr::core::vector3d<float> pos/*, Player player*/)
{
	position = pos;
	health = 100;
	damage = 25;
	maxDistance = 2;

	selected = false;
/*	if(player.playerNumber == 1)
		player1 = true;
	else
		player1 = false'*/
}


BaseUnit::~BaseUnit(void)
{
}

void BaseUnit::MoveBack(float distance){
	if(distance > maxDistance)
		distance = maxDistance;

	position.Z -= distance;
	return;
}

void BaseUnit::MoveForward(float distance){
	if(distance > maxDistance)
		distance = maxDistance;

	position.Z += distance;
	return;
}

void BaseUnit::MoveLeft(float distance){
	if(distance > maxDistance)
		distance = maxDistance;
	
	position.X -= distance;
	return;
}

void BaseUnit::MoveRight(float distance){
	if(distance > maxDistance)
		distance = maxDistance;
	
	position.X += distance;
	return;
}

void BaseUnit::SelectUnit(){
	if(selected)
		selected = !selected;
	else
		selected = true;

}
