#include "BaseUnit.h"

// there needs to be a player given in the constructor as parameter soo the player1 boolean can be set for the unit 
// this way we can keep track of which unit is from which player.
BaseUnit::BaseUnit(irr::core::vector3d<float> pos, bool player, IrrlichtDevice* dev, int id)
{
	this->id = id;
	position = pos;
	health = 100;
	damage = 25;
	maxDistance = 1;

	selected = false;

    device = dev;
    sceneManager = device->getSceneManager();
    driver = device->getVideoDriver();
	player1 = player;

    meshPath = "../media/testunit.irrmesh";
	if(player1){
		texturePath = "../media/Unit_P1.jpg";
		texturePathHighlight = "../media/Unit_P1_Highlight.jpg";
		texturePathSelected = "../media/Unit_P1_Selected.jpg";
	}else{
		texturePath = "../media/Unit_P2.jpg";
		texturePathHighlight = "../media/Unit_P2_Highlight.jpg";
		texturePathSelected = "../media/Unit_P2_Selected.jpg";
	}
	createMesh();

	//this->initUnitHighLight();
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


	cout << "the unit: " << position.X << ", " << position.Y << ", " << position.Z;
}

void BaseUnit::SelectUnit(){
	selected = !selected;
	if(selected)
		node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
	else if(!selected)
		node->setMaterialTexture(0, driver->getTexture(texturePath));
//	if(selected)
//		selected = !selected;
//	else
//		selected = true;

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
    IAnimatedMesh* mesh = sceneManager->getMesh(meshPath);
    
    if (!mesh){
        cout << "The mesh could not be created in MashViewer->createMesh";
        return false;
    }
    node = sceneManager->addAnimatedMeshSceneNode( mesh );
    
    if (node)
    {
        float scale = 0.5;
		vector3d<float> temp = vector3d<float>((1-scale)/2, 0 , (1-scale)/2);
		position += temp;
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture(texturePath) );
        node->setPosition(position);
        node->setScale(core::vector3df(scale,scale,scale));
    }
	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "unit mesh bounding box X: " << extent.X << " Y: " << extent.Y << " Z: " << extent.Z << endl;
    return true;
}

void BaseUnit::highLightUnit(bool highLight)
{
	//selectIndication = NULL;
	if(highLight)
	{
		//Move(FORWARD, 2);
		if(player1)
			node->setMaterialTexture(0, driver->getTexture(texturePathHighlight));
		else
			node->setMaterialTexture(0, driver->getTexture(texturePathHighlight));
		//selectIndication->setVisible(true);
		//indicationBoard->setVisible(true);
	}
	else
	{
		if(selected)
			node->setMaterialTexture(0, driver->getTexture(texturePathSelected));
		else if(!selected)
		{
			node->setMaterialTexture(0, driver->getTexture(texturePath));
			//selectIndication->setVisible(false);
			//indicationBoard->setVisible(false);
		}
	}
}

//void BaseUnit::initUnitHighLight()
//{
//	vector3d<f32> newPosition = this->position + vector3d<f32>(0,0.5,0);
//	selectIndication =
//		sceneManager->addLightSceneNode(0, position,
//		video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 800.0f);
//	selectIndication->setDebugDataVisible ( scene::EDS_BBOX );
//
//	selectIndication->setVisible(false);
//
//	// attach billboard to the light
//	indicationBoard =
//		sceneManager->addBillboardSceneNode(selectIndication, core::dimension2d<f32>(1, 1));
//
//	indicationBoard->setMaterialFlag(video::EMF_LIGHTING, false);
//	indicationBoard->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
//	indicationBoard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
//	indicationBoard->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/particlegreen.jpg"));
//
//	indicationBoard->setVisible(false);
//}
