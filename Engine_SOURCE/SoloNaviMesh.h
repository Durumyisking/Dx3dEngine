#pragma once
#include "NaviMesh.h"

class Mesh;
class Model;

class SoloNaviMesh :
    public NaviMesh
{
public:
    SoloNaviMesh();
    virtual ~SoloNaviMesh();

	bool Build();

	bool SettingMesh(Mesh* mesh);
	bool SettingMeshes(const std::vector<Mesh>& meshes);
	bool SettingModel(Model* model);

	void Clear();

private:
	bool mKeepInterResults;
	unsigned char* mTriAreas;
	rcHeightfield* mHeightField;
	rcCompactHeightfield* mCompactHeightField;
	rcContourSet* mContourSet;
	rcPolyMesh* mPolyMesh;
	rcConfig mConfig;
	rcPolyMeshDetail* mDetailMesh;
};

