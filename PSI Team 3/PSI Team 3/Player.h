#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "BaseUnit.h"
#include <fstream>

class Player
{
private:
	bool player1;
	std::vector<BaseUnit*>* units;
	

public:
	Player(IrrlichtDevice* dev);
	~Player(void);

	irr::core::vector3d<float> basePosition;

	std::vector<BaseUnit*>* getUnits() { return units; }
	bool getPlayer1() { return player1; }
	IrrlichtDevice* device;

	// void setUnits(std::vector<BaseUnit>* units) { this->units = units; }
	void initUnits();
	void setPlayer1(bool player1) { this->player1 = player1; }
	void setDevice(IrrlichtDevice* dev);
};

#endif