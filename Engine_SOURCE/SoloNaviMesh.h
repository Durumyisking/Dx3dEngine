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

	bool SettingMesh(const std::wstring& name, GameObj* obj);

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

