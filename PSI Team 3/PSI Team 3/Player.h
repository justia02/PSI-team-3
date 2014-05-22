#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Mover.h"
#include "Shooter.h"
#include "BalancedUnit.h"
#include <fstream>

class Player
{
private:
	bool player1;
	int actionsLeft;
	std::vector<BaseUnit*>* units;
	

public:
	Player(IrrlichtDevice* dev);
	~Player(void);
	irr::core::vector3d<float>* basePositions;

	std::vector<BaseUnit*>* getUnits() { return units; }
	int getActionsLeft() { return actionsLeft; }
	void setActionsLeft();// { actionsLeft = 4; }
	void resetActionsLeft();
	bool getPlayer1() { return player1; }
	bool actionAllowed();
	IrrlichtDevice* device;

	// void setUnits(std::vector<BaseUnit>* units) { this->units = units; }
	void initUnits();
	void setPlayer1(bool player1) { this->player1 = player1; }
	void setDevice(IrrlichtDevice* dev);
};

#endif