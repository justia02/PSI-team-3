#include "BaseUnit.h"

class Mover : public BaseUnit {

public:
	Mover(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);

private:
	bool createMesh();
	virtual void SelectUnit();
	virtual void highLightUnit(bool highLight);
	IAnimatedMesh* normalMesh;
	IAnimatedMesh* selectedMesh;
	IAnimatedMesh* highlightedMesh;

};