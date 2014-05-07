#include "BaseUnit.h"

class Shooter : public BaseUnit {

	Shooter() : BaseUnit() {

		health = 100;
		damage = 30;
		maxDistance = 1;
		shootingRange = 3;

	};

};