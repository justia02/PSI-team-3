#include "Player.h"
#include "irrlicht.h"

using namespace std;

Player::Player(IrrlichtDevice* dev){
	device = dev;
	units = new std::vector<BaseUnit*>;
	basePositions = new irr::core::vector3d<float>[2];
	actionsLeft = -1;
}

Player::~Player()
{
	delete units;
}

void Player::initUnits() {
	ifstream infile("../assets/positions.txt");
	float x, y, z, pl, type;
	int id = 0;
	int temp = 0;
	
	while(infile >> x >> y >> z >> pl >> type){
		if(type == 0){
			if(pl == 1 && player1){
				units->push_back(new BaseUnit(vector3d<float>(x, y, z), getPlayer1(), device, id));
			}
			else if(pl == 2 && !player1){
				units->push_back(new BaseUnit(vector3d<float>(x, y, z), getPlayer1(), device, id));
			}
			id++;
		}
		else if(type == 4){
			if(pl == 1 && player1){
				basePositions[temp] = vector3d<float>(x, y, z);
				temp++;
			}
			else if(pl == 2 && !player1){
				basePositions[temp] = vector3d<float>(x, y, z);
				temp++;
			}
		}
	}
}

void Player::setDevice(IrrlichtDevice* dev){
	device = dev;
}

bool Player::actionAllowed(){
	if(actionsLeft > 0)
		return true;
	else
		return false;
}

void Player::setActionsLeft(){
	if(actionsLeft > 0){
		actionsLeft -= 1;
	}
	else if(actionsLeft <= 0){
		actionsLeft = 4;
	}
	cout << "Actionsleft: " << actionsLeft << endl;
}