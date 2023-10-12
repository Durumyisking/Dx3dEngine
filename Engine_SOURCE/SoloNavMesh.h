#pragma once
#include "Entity.h"
#include "../External/Recast/include/Recast.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"

class Model;
class SoloNavMesh :
    public Entity
{
public:
	SoloNavMesh();
	SoloNavMesh(const SoloNavMesh& other);
	virtual ~SoloNavMesh();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	bool CreateByModel(Model* model);

private:
	rcPolyMeshDetail*	mDetailMesh;
	rcPolyMesh*			mPolyMesh;
};

