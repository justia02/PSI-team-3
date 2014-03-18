#include "MeshViewer.h"

//using namespace irr;

//using namespace scene;

MeshViewer::MeshViewer(coreIO::path md2Path, coreIO::path bmpPath)
{
	MeshViewer::md2Path = md2Path;
	MeshViewer::bmpPath = bmpPath;
}


MeshViewer::~MeshViewer(void)
{
}

void MeshViewer::setShininess(float value)
{
	node->getMaterial(0).Shininess = value;
}

//void MeshViewer::setSceneManager(ISceneManager* test)
void MeshViewer::setSceneManager(coreScene::ISceneManager* smng)
{
	MeshViewer::sceneManager = smng;
}

void MeshViewer::setDevice(IrrlichtDevice* device)
{
	MeshViewer::device = device;
}

void MeshViewer::setDriver(IVideoDriver* driver)
{
	MeshViewer::driver = driver;
}

void MeshViewer::setNewPosition(core::vector3d<float> position)
{
	if(MeshViewer::node == NULL)
	{
		cout << "The node is not set. The position cant be changed";
		return;
	}
	node->setPosition(position);
}

bool MeshViewer::createMesh()
{
	if(MeshViewer::device == NULL)
	{
		cout << "The device is not set in MeshViewer->createMesh";
		return false;
	}
	if(MeshViewer::sceneManager == NULL)
	{
		cout << "The Scene manager is not set in MeshViewer->createMesh";
		return false;
	}
	if(MeshViewer::driver == NULL)
	{
		cout << "The driver is not set in MeshViewer->createMesh";
		return false;
	}

	coreScene::IAnimatedMesh* mesh = sceneManager->getMesh(md2Path);
	if (!mesh)
	{
		cout << "The mesh could not be created in MashViewer->createMesh";
		return false;
	}
	//coreScene::IAnimatedMeshSceneNode* node = sceneManager->addAnimatedMeshSceneNode( mesh );
	node = sceneManager->addAnimatedMeshSceneNode( mesh );


	if (node)
	{
		float scale = 0.25;

		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture(bmpPath) );
		//node->setPosition(core::vector3df(-1.8,4,-5.7));	// close left
		//node->setPosition(core::vector3df(1.8,4,-5.7));	// close right
		//node->setPosition(core::vector3df(1.8,4,-2.2));	// far right
		node->setPosition(core::vector3df(-1.8,4,-2.2));	// far left
		node->setScale(core::vector3df(scale,scale,scale));
	}

	return true;
}
