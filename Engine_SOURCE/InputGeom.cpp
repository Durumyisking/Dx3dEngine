#include "InputGeom.h"
#include "NaviMesh.h"
#include "PathMgr.h"
#include "Mesh.h"
#include "Model.h"
#include "GameObj.h"
#include "MeshRenderer.h"
//#include "Recast.h"
//#include "DetourNavMesh.h"

InputGeom::InputGeom()
	: mbHasBuildSettings(false)
	, mOffMeshConCount(0)
	, mVerts{}
	, mTriangles{}
{
}

InputGeom::~InputGeom()
{
	mVerts.clear();
	mTriangles.clear();
}

bool InputGeom::CreateGeom(const std::wstring& name, GameObj* obj)
{
	if (obj == nullptr)
		return false;

	Transform* tr = obj->GetComponent<Transform>();
	if (tr == nullptr)
		return false;

	MeshRenderer* mr = obj->GetComponent<MeshRenderer>();
	if (mr == nullptr)
		return false;

	math::Matrix world = tr->GetWorldMatrix();

	if (mr->GetModel() == nullptr)
	{
		if (CalcMeshVertexes(world, mr->GetMesh()))
		{
			mGeomName = name;
			return true;
		}
	}
	else
	{
		if (CalcModelVertex(world, mr->GetModel()))
		{
			mGeomName = name;
			return true;
		}
	}

	return false;
}

bool InputGeom::CalcModelVertex(math::Matrix world, Model* model)
{
	if (model == nullptr)
		return false;

	//vertexCount = model->GetNumberOfVertices(index);
	const std::vector<Mesh*> meshes = model->GetMeshes();

	if (meshes.empty())
		return false;

	int allVertexCount = 0;
	int allIndexCount = 0;
	int currentIndexOffset = 0;

	for (Mesh* mesh : meshes)
	{
		std::vector<Vertex> meshVertices;
		std::vector<UINT> meshIndexes;
		mesh->GetVerticesFromBuffer(&meshVertices);
		mesh->GetIndexesFromBuffer(&meshIndexes);
		int vertexCount = static_cast<int>(mesh->GetVertexCount());
		int indexCount = static_cast<int>(mesh->GetIndexCount());
		allVertexCount += vertexCount;
		allIndexCount += indexCount;

		// Copy from cvector array to PxVec3 array
		for (int i = 0; i < vertexCount; i++)
		{
			Vector4 transformedVertex = DirectX::XMVector4Transform(meshVertices[i].pos, world);

			mVerts.push_back(transformedVertex.x);
			mVerts.push_back(transformedVertex.y);
			mVerts.push_back(transformedVertex.z);
		}

		for (int i = 0; i < indexCount; i++)
		{
			mTriangles.emplace_back(meshIndexes[i] + currentIndexOffset);
		}

		currentIndexOffset += vertexCount;
	}

	mNumVerts = allVertexCount;
	if (allIndexCount % 3 != 0)
		int debug = 0;
	mNumTriangles = allIndexCount / 3;

	rcCalcBounds(mVerts.data(), mNumVerts, mMeshBMin, mMeshBMax);

	return true;
}

bool InputGeom::CalcMeshVertexes(math::Matrix world, Mesh* mesh)
{
	if (mesh == nullptr)
		return false;


	std::vector<Vertex> meshVertices;
	std::vector<UINT> meshIndexes;
	mesh->GetVerticesFromBuffer(&meshVertices);
	mesh->GetIndexesFromBuffer(&meshIndexes);
	int vertexCount = static_cast<int>(mesh->GetVertexCount());
	int indexCount = static_cast<int>(mesh->GetIndexCount());

	// Copy from cvector array to PxVec3 array
	for (int i = 0; i < vertexCount; i++)
	{
		Vector4 transformedVertex = DirectX::XMVector4Transform(meshVertices[i].pos, world);

		mVerts.push_back(transformedVertex.x);
		mVerts.push_back(transformedVertex.y);
		mVerts.push_back(transformedVertex.z);
	}

	for (int i = 0; i < indexCount; i++)
	{
		mTriangles.emplace_back(meshIndexes[i]);
	}

	mNumVerts = vertexCount;
	if (indexCount % 3 != 0)
		int debug = 0;
	mNumTriangles = indexCount / 3;

	rcCalcBounds(mVerts.data(), mNumVerts, mMeshBMin, mMeshBMax);

	return true;
}

void InputGeom::AddOffMeshConnection(const float* spos, const float* epos, const float rad,
	unsigned char bidir, unsigned char area, unsigned short flags)
{
	if (mOffMeshConCount >= MAX_OFFMESH_CONNECTIONS) return;
	float* v = &mOffMeshConVerts[mOffMeshConCount * 3 * 2];
	mOffMeshConRads[mOffMeshConCount] = rad;
	mOffMeshConDirs[mOffMeshConCount] = bidir;
	mOffMeshConAreas[mOffMeshConCount] = area;
	mOffMeshConFlags[mOffMeshConCount] = flags;
	mOffMeshConId[mOffMeshConCount] = 1000 + mOffMeshConCount;
	rcVcopy(&v[0], spos);
	rcVcopy(&v[3], epos);
	mOffMeshConCount++;
}

void InputGeom::DeleteOffMeshConnection(int i)
{
	mOffMeshConCount--;
	float* src = &mOffMeshConVerts[mOffMeshConCount * 3 * 2];
	float* dst = &mOffMeshConVerts[i * 3 * 2];
	rcVcopy(&dst[0], &src[0]);
	rcVcopy(&dst[3], &src[3]);
	mOffMeshConRads[i] = mOffMeshConRads[mOffMeshConCount];
	mOffMeshConDirs[i] = mOffMeshConDirs[mOffMeshConCount];
	mOffMeshConAreas[i] = mOffMeshConAreas[mOffMeshConCount];
	mOffMeshConFlags[i] = mOffMeshConFlags[mOffMeshConCount];
}
