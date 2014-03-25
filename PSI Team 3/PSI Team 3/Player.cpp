#include "Player.h"
#include "irrlicht.h"

using namespace std;

Player::Player(IrrlichtDevice* dev){
	device = dev;
	units = new std::vector<BaseUnit>;
}

Player::~Player()
{
	delete units;
}

void Player::initUnits() {
	ifstream infile("../assets/positions.txt");
	float x, y, z, pl;
	while(infile >> x >> y >> z >> pl){
		if(pl == 1 && player1){
			units->push_back(BaseUnit(vector3d<float>(x, y, z), player1, device));
		}
		else if(pl == 2 && !player1){
			units->push_back(BaseUnit(vector3d<float>(x, y, z), player1, device));
		}
	}
}
