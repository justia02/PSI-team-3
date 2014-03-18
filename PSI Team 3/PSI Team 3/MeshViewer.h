#pragma once

#include "string";
#include "irrlicht.h"
#include "iostream";

using namespace std;
using namespace irr;

using namespace video;

namespace coreScene = irr::scene;
namespace coreIO = irr::io;
namespace coreIrr = irr;

class MeshViewer
{
public:
	MeshViewer(coreIO::path md2Path, coreIO::path bmpPath);
	~MeshViewer(void);

	void setSceneManager(coreScene::ISceneManager* smng);
	void setDevice(coreIrr::IrrlichtDevice* device);
	void setDriver(IVideoDriver* driver);
	bool createMesh();

	void setNewPosition(core::vector3d<float> position);
	void setShininess(float value);

private:
	coreScene::ISceneManager* sceneManager;
	coreIO::path md2Path;
	coreIO::path bmpPath;
	coreIrr::IrrlichtDevice* device;
	coreScene::IAnimatedMeshSceneNode* node;
	IVideoDriver* driver;
};

