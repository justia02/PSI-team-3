#include "Player.h"
#include "irrlicht.h"

using namespace std;

Player::Player(IrrlichtDevice* dev){
	device = dev;
	units = new std::vector<BaseUnit>;
	initUnits();
}

Player::~Player()
{
	delete units;
}

void Player::initUnits() {
	ifstream infile("../assets/positions.txt");
	float x, y, z, pl;
	while(infile >> x >> y >> z >> pl){
		if(pl == 1)
			setPlayer1(true);
		else
			setPlayer1(false);
		units->push_back(BaseUnit(vector3d<float>(x, y, z), player1, device));
	}
}
