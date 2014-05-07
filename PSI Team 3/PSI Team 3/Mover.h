#include "BaseUnit.h"

class Mover : public BaseUnit {

	Mover() : BaseUnit() {

		health = 100;
		damage = 20;
		maxDistance = 3;
		shootingRange = 1;

	};

};