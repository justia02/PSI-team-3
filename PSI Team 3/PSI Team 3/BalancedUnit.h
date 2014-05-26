#include "BaseUnit.h"

class BalancedUnit : public BaseUnit {

private:
	virtual bool createMesh();

public:
	BalancedUnit(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);
	virtual void setPosition(vector3df position);

};