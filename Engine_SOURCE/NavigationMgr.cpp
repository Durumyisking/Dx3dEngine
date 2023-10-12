#include "NavigationMgr.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Model.h"

NavigationMgr::NavigationMgr()
{
}

NavigationMgr::~NavigationMgr()
{
}

void NavigationMgr::Update()
{
}

void NavigationMgr::FixedUpdate()
{
}

void NavigationMgr::CreateNavigationMesh(Model* model)
{
	//vertexCount = model->GetNumberOfVertices(index);
	const std::vector<Mesh*> meshes = model->GetMeshes();
	std::vector<math::Vector3> vertices;
	std::vector<int> indexes;
	int allVertexCount = 0;
	int allIndexCount = 0;
	int currentIndexOffset = 0;

	for (Mesh* mesh : meshes)
	{
		std::vector<Vertex> meshVertices;
		std::vector<UINT> meshIndexes;
		mesh->GetVerticesFromBuffer(&meshVertices);
		mesh->GetIndexesFromBuffer(&meshIndexes);
		int vertexCount = mesh->GetVertexCount();
		int indexCount = mesh->GetIndexCount();
		allVertexCount += vertexCount;
		allIndexCount += indexCount;

		// Copy from cvector array to PxVec3 array
		for (int i = 0; i < vertexCount; i++)
		{
			vertices.emplace_back(meshVertices[i].pos.x, meshVertices[i].pos.y, meshVertices[i].pos.z);
		}

		for (int i = 0; i < indexCount; i++)
		{
			indexes.emplace_back(static_cast<int>(meshIndexes[i]) + currentIndexOffset);
		}

		currentIndexOffset += vertexCount;
	}
}

void NavigationMgr::SetRecast(math::Vector3* verts, int vertCount, int* indices, int indexCount)
{
}

void NavigationMgr::Navigation()
{
}
