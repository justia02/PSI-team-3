#include "BaseUnit.h"

class Mover : public BaseUnit {

public:
	Mover(vector3d<float> position, bool player1, IrrlichtDevice* dev, int id);
	virtual void setPosition(vector3df position);
	virtual void SelectUnit();
	virtual void highLightUnit(bool highLight);

private:
	bool createMesh();
	IAnimatedMesh* normalMesh;
	IAnimatedMesh* selectedMesh;
	IAnimatedMesh* highlightedMesh;

};