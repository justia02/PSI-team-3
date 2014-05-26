#include "BaseUnit.h"

class Shooter : public BaseUnit {

public:
	Shooter(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);
	virtual void setPosition(vector3df position);

private:
	bool createMesh();

};