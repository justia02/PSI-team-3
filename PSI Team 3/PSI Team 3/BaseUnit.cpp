#include "BaseUnit.h"

// there needs to be a player given in the constructor as parameter soo the player1 boolean can be set for the unit 
// this way we can keep track of which unit is from which player.
BaseUnit::BaseUnit(irr::core::vector3d<float> pos, bool player, IrrlichtDevice* dev)
{
	position = pos;
	health = 100;
	damage = 25;
	maxDistance = 2;

	selected = false;

    device = dev;
    sceneManager = device->getSceneManager();
    driver = device->getVideoDriver();

    md2Path = "../media/testunit.irrmesh";
    createMesh();
	player1 = player;
}


BaseUnit::~BaseUnit(void)
{
}

void BaseUnit::Move(direction moveDirection, float distance){
	if(distance > maxDistance)
		distance = maxDistance;
	switch(moveDirection){
		case LEFT:{
			position.X -= distance;
			break;
			}
		case RIGHT:{
			position.X += distance;
			break;
			}
		case BACK:{
			position.Z -= distance;
			break;
			}
		case FORWARD:{
			position.Z += distance;
			break;
			}
		}
	node->setPosition(position);
}

void BaseUnit::SelectUnit(){
	if(selected)
		selected = !selected;
	else
		selected = true;

}

void BaseUnit::setShininess(float value){
    node->getMaterial(0).Shininess = value;
}

bool BaseUnit::createMesh(){
    if(device == NULL){
        cout << "The device is not set in MeshViewer->createMesh";
        return false;
    }
    
    if(sceneManager == NULL){
        cout << "The Scene manager is not set in MeshViewer->createMesh";
        return false;
    }
    
    if(driver == NULL){
        cout << "The driver is not set in MeshViewer->createMesh";
        return false;
    }
    IAnimatedMesh* mesh = sceneManager->getMesh(md2Path);
    
    if (!mesh){
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }
    node = sceneManager->addAnimatedMeshSceneNode( mesh );
    
    if (node)
    {
        float scale = 0.25;

        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        //node->setMaterialTexture( 0, driver->getTexture(bmpPath) );
        node->setPosition(position);
        node->setScale(core::vector3df(scale,scale,scale));
    }
    return true;
}
