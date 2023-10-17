#pragma once
#include "NaviMesh.h"

class Mesh;
class Model;
class GameObj;

class SoloNaviMesh :
    public NaviMesh
{
public:
    SoloNaviMesh();
    virtual ~SoloNaviMesh();

	void HandleSettings();

	void saveAll();
	void loadAll();

	bool Build();

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

