#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "BaseUnit.h"

class Player
{
private:
	bool player1;
	std::vector<BaseUnit>* units;
public:
	Player(void);
	~Player(void);
	void initUnits();
	std::vector<BaseUnit>* getUnits() { return units; }
	bool getPlayer1() { return player1; }
	// void setUnits(std::vector<BaseUnit>* units) { this->units = units; }
	void setPlayer1(bool player1) { this->player1 = player1; }
};

#endif