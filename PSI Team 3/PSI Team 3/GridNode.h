#include <irrlicht.h>

using namespace video;
using namespace io;
using namespace core;

class GridNode {

	private:
		path texturePath;
		path textureHighLightPath;
		ISceneManager* smgr;

	public:
		GridNode(ISceneManager* smgr, IAnimatedMesh* mesh, vector3df position);
		irr::core::vector3df position;
		IAnimatedMeshSceneNode* node;
		bool highlighted;

		void changeTexture();

};