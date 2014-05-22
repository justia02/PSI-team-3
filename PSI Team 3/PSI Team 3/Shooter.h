#include "BaseUnit.h"

class Shooter : public BaseUnit {

public:
	Shooter(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);

private:
	bool createMesh();

};