#include "BaseUnit.h"

class Mover : public BaseUnit {

public:
	Mover(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);

private:
	bool createMesh();

};