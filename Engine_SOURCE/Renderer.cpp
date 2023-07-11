#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "PaintShader.h"
#include "ParticleShader.h"
#include "TimeMgr.h"
#include "Application.h"
#include "AudioClip.h"

namespace dru::renderer
{
	

	ConstantBuffer* constantBuffers[static_cast<UINT>(eCBType::End)] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[static_cast<UINT>(dru::eSamplerType::End)];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState[static_cast<UINT>(dru::eRasterizerType::End)];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[static_cast<UINT>(dru::eDepthStencilType::End)];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState[static_cast<UINT>(dru::eBlendStateType::End)];

	Camera* mainCamera = nullptr;
	std::vector<Camera*> Cameras[static_cast<UINT>(SceneMgr::eSceneType::End)];
	std::vector<DebugMesh> debugMeshes;
	std::vector<LightAttribute> lights;
	StructedBuffer* lightBuffer = nullptr;

	Texture* postProcessTexture = nullptr;
	dru::GameObj* inspectorGameObject = nullptr;

	void LoadMesh()
	{

#pragma region PointMesh

		Vertex PointVertex = {};
		Mesh* pointMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Pointmesh", pointMesh);
		pointMesh->CreateVertexBuffer(&PointVertex, 1);
		UINT pointIndex = 0;
		pointMesh->CreateIndexBuffer(&pointIndex, 1);

#pragma endregion

#pragma region LineMesh

		Vertex LineVertex[2] = {};

		LineVertex[0].pos = Vector4(-0.5f, 0.25f, 0.f, 1.f); 
		LineVertex[0].color = Vector4(0.f, 1.f, 0.f, 1.f); 
		LineVertex[0].uv = Vector2(0.f, 0.f); 

		LineVertex[1].pos = Vector4(0.5f, 0.25f, 0.f, 1.f);
		LineVertex[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		LineVertex[1].uv = Vector2(1.f, 0.f);


		Mesh* lineMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Linemesh", lineMesh);
		lineMesh->CreateVertexBuffer(&LineVertex, 2);
		std::vector<UINT> lineindexes;
		lineindexes.push_back(0);
		lineindexes.push_back(1);
		lineMesh->CreateIndexBuffer(lineindexes.data(), static_cast<UINT>(lineindexes.size()));

#pragma endregion

#pragma region RectMesh

		Vertex	RectVertexes[4] = {};

		RectVertexes[0].pos = Vector4(-0.5f, 0.5f, 0.f, 1.f); 
		RectVertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f); 
		RectVertexes[0].uv = Vector2(0.f, 0.f); 

		RectVertexes[1].pos = Vector4(0.5f, 0.5f, 0.f, 1.f);
		RectVertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		RectVertexes[1].uv = Vector2(1.f, 0.f);

		RectVertexes[2].pos = Vector4(0.5f, -0.5f, 0.f, 1.f);
		RectVertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		RectVertexes[2].uv = Vector2(1.f, 1.f);

		RectVertexes[3].pos = Vector4(-0.5f, -0.5f, 0.f, 1.f);
		RectVertexes[3].color = Vector4(0.f, 0.f, 0.f, 1.f);
		RectVertexes[3].uv = Vector2(0.f, 1.f);

		Mesh* Rectmesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Rectmesh", Rectmesh);
		Rectmesh->CreateVertexBuffer(RectVertexes, 4);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		Rectmesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

#pragma endregion

#pragma region GridMesh

		Vertex	GridVertexes[4] = {};

		GridVertexes[0].pos = Vector4(-200.f, 200.f, 0.f, 1.f);
		GridVertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		GridVertexes[0].uv = Vector2(0.f, 0.f);

		GridVertexes[1].pos = Vector4(200.f, 200.f, 0.f, 1.f);
		GridVertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		GridVertexes[1].uv = Vector2(1.f, 0.f);

		GridVertexes[2].pos = Vector4(200.f, -200.f, 0.f, 1.f);
		GridVertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		GridVertexes[2].uv = Vector2(1.f, 1.f);

		GridVertexes[3].pos = Vector4(-200.f, -200.f, 0.f, 1.f);
		GridVertexes[3].color = Vector4(0.f, 0.f, 0.f, 1.f);
		GridVertexes[3].uv = Vector2(0.f, 1.f);

		Mesh* Gridmesh = new Mesh();
		dru::GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Gridmesh", Gridmesh);
		Gridmesh->CreateVertexBuffer(GridVertexes, 4);

		indexes.clear();

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		Gridmesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

#pragma endregion

#pragma region RectMesh_Debug

		Vertex	DebugRectVertexes[4] = {};

		DebugRectVertexes[0].pos = Vector4(-0.5f, 0.5f, -0.00001f, 1.f); 
		DebugRectVertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f); 
		DebugRectVertexes[0].uv = Vector2(0.f, 0.f); 

		DebugRectVertexes[1].pos = Vector4(0.5f, 0.5f, -0.00001f, 1.f);
		DebugRectVertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		DebugRectVertexes[1].uv = Vector2(1.f, 0.f);

		DebugRectVertexes[2].pos = Vector4(0.5f, -0.5f, -0.00001f, 1.f);
		DebugRectVertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		DebugRectVertexes[2].uv = Vector2(1.f, 1.f);

		DebugRectVertexes[3].pos = Vector4(-0.5f, -0.5f, -0.00001f, 1.f);
		DebugRectVertexes[3].color = Vector4(0.f, 0.f, 0.f, 1.f);
		DebugRectVertexes[3].uv = Vector2(0.f, 1.f);

		indexes.clear();
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);

		Mesh* DebugRectmesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"DebugRectmesh", DebugRectmesh);
		DebugRectmesh->CreateVertexBuffer(DebugRectVertexes, 4);
		DebugRectmesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

#pragma endregion

#pragma region CircleMesh

		std::vector<Vertex>	CircleVertexes;
		Vertex center = {};
		center.pos = Vector4(0.f, 0.f, -0.00001f, 1.f);
		center.color = Vector4(0.f, 1.f, 0.f, 1.f);
		center.uv = Vector2::Zero;

		CircleVertexes.push_back(center);

		int slice = 80;
		float radius = 0.5f;
		float theta = XM_2PI / (float)slice;

		for (int i = 0; i < slice; i++)
		{
			Vertex vtx = {};
			vtx.pos = Vector4(radius * cosf(theta * (float)i)
				, radius * sinf(theta * (float)i)
				, -0.00001f, 1.f
			);
			vtx.color = center.color;

			CircleVertexes.push_back(vtx);
		}
		indexes.clear();
		for (int i = 1; i <= slice; i++) 
		{
			indexes.push_back(i);
		}
		indexes.push_back(1);

		Mesh* Circlemesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Circlemesh", Circlemesh);
		Circlemesh->CreateVertexBuffer(CircleVertexes.data(), static_cast<UINT>(CircleVertexes.size()));
		Circlemesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));


#pragma endregion

#pragma region Cube Mesh
		Vertex arrCube[24] = {};

		//struct Vertex
		//{
		//	Vector4 pos;
		//	Vector4 color;
		//	Vector2 uv;
		//};

		// ����
		arrCube[0].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[0].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[0].uv = Vector2(0.f, 0.f);
		arrCube[0].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[1].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[1].uv = Vector2(0.f, 0.f);
		arrCube[1].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[2].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[2].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[2].uv = Vector2(0.f, 0.f);
		arrCube[2].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[3].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[3].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[3].uv = Vector2(0.f, 0.f);
		arrCube[3].normal = Vector3(0.f, 1.f, 0.f);


		// �Ʒ� ��	
		arrCube[4].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[4].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[4].uv = Vector2(0.f, 0.f);
		arrCube[4].normal = Vector3(0.f, -1.f, 0.f);

		arrCube[5].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[5].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[5].uv = Vector2(0.f, 0.f);
		arrCube[5].normal = Vector3(0.f, -1.f, 0.f);

		arrCube[6].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[6].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[6].uv = Vector2(0.f, 0.f);
		arrCube[6].normal = Vector3(0.f, -1.f, 0.f);

		arrCube[7].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[7].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[7].uv = Vector2(0.f, 0.f);
		arrCube[7].normal = Vector3(0.f, -1.f, 0.f);

		// ���� ��
		arrCube[8].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[8].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[8].uv = Vector2(0.f, 0.f);
		arrCube[8].normal = Vector3(-1.f, 0.f, 0.f);

		arrCube[9].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[9].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[9].uv = Vector2(0.f, 0.f);
		arrCube[9].normal = Vector3(-1.f, 0.f, 0.f);

		arrCube[10].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[10].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[10].uv = Vector2(0.f, 0.f);
		arrCube[10].normal = Vector3(-1.f, 0.f, 0.f);

		arrCube[11].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[11].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[11].uv = Vector2(0.f, 0.f);
		arrCube[11].normal = Vector3(-1.f, 0.f, 0.f);

		// ������ ��
		arrCube[12].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[12].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[12].uv = Vector2(0.f, 0.f);
		arrCube[12].normal = Vector3(1.f, 0.f, 0.f);

		arrCube[13].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[13].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[13].uv = Vector2(0.f, 0.f);
		arrCube[13].normal = Vector3(1.f, 0.f, 0.f);

		arrCube[14].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[14].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[14].uv = Vector2(0.f, 0.f);
		arrCube[14].normal = Vector3(1.f, 0.f, 0.f);

		arrCube[15].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[15].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[15].uv = Vector2(0.f, 0.f);
		arrCube[15].normal = Vector3(1.f, 0.f, 0.f);

		// �� ��
		arrCube[16].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[16].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[16].uv = Vector2(0.f, 0.f);
		arrCube[16].normal = Vector3(0.f, 0.f, 1.f);

		arrCube[17].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[17].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[17].uv = Vector2(0.f, 0.f);
		arrCube[17].normal = Vector3(0.f, 0.f, 1.f);

		arrCube[18].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[18].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[18].uv = Vector2(0.f, 0.f);
		arrCube[18].normal = Vector3(0.f, 0.f, 1.f);

		arrCube[19].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[19].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[19].uv = Vector2(0.f, 0.f);
		arrCube[19].normal = Vector3(0.f, 0.f, 1.f);

		// �� ��
		arrCube[20].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);;
		arrCube[20].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[20].uv = Vector2(0.f, 0.f);
		arrCube[20].normal = Vector3(0.f, 0.f, -1.f);

		arrCube[21].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[21].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[21].uv = Vector2(0.f, 0.f);
		arrCube[21].normal = Vector3(0.f, 0.f, -1.f);

		arrCube[22].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[22].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[22].uv = Vector2(0.f, 0.f);
		arrCube[22].normal = Vector3(0.f, 0.f, -1.f);

		arrCube[23].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[23].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[23].uv = Vector2(0.f, 0.f);
		arrCube[23].normal = Vector3(0.f, 0.f, -1.f);

		indexes.clear();
		for (int i = 0; i < 6; i++)
		{
			indexes.push_back(i * 4);
			indexes.push_back(i * 4 + 1);
			indexes.push_back(i * 4 + 2);

			indexes.push_back(i * 4);
			indexes.push_back(i * 4 + 2);
			indexes.push_back(i * 4 + 3);
		}

		// Crate Mesh
		Mesh* cubMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Cubemesh", cubMesh);
		cubMesh->CreateVertexBuffer(arrCube, 24);
		cubMesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));
#pragma endregion

#pragma region Sphere Mesh

		Vertex v = {};
		float fRadius = 0.5f;
		std::vector<Vertex> sphereVtx;

		// Top
		v.pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
		v.uv = Vector2(0.5f, 0.f);
		v.color = Vector4(1.f, 1.f, 1.f, 1.f);
		v.normal = Vector3(0.0f, fRadius, 0.0f);
		v.normal.Normalize();
		v.tangent = Vector3(1.f, 0.f, 0.f);
		v.biNormal = Vector3(0.f, 0.f, 1.f);

		sphereVtx.push_back(v);

		// Body
		UINT iStackCount = 40; // ���� ���� ����
		UINT iSliceCount = 40; // ���� ���� ����

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.pos = Vector4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
				v.uv = Vector2(fUVXStep * j, fUVYStep * i);
				v.color = Vector4(1.f, 1.f, 1.f, 1.f);
				v.normal = Vector3(v.pos.x, v.pos.y, v.pos.z);
				//v.normal.Normalize();

				v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.f;
				v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				v.tangent.Normalize();

				v.tangent.Cross(v.normal, v.biNormal);
				v.biNormal.Normalize();

				sphereVtx.push_back(v);
			}
		}

		// Bottom
		v.pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
		v.uv = Vector2(0.5f, 1.f);
		v.color = Vector4(1.f, 1.f, 1.f, 1.f);
		v.normal = Vector3(0.0f, -fRadius, 0.0f);
		v.normal.Normalize();

		v.tangent = Vector3(1.f, 0.f, 0.f);
		v.biNormal = Vector3(0.f, 0.f, -1.f);
		sphereVtx.push_back(v);

		// �ε���
		// �ϱ���
		indexes.clear();
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indexes.push_back(0);
			indexes.push_back(i + 2);
			indexes.push_back(i + 1);
		}

		// ����
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				indexes.push_back((iSliceCount + 1) * (i)+(j)+1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				indexes.push_back((iSliceCount + 1) * (i)+(j)+1);
				indexes.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// ������
		UINT iBottomIdx = static_cast<UINT>(sphereVtx.size()) - 1;

		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indexes.push_back(iBottomIdx);
			indexes.push_back(iBottomIdx - (i + 2));
			indexes.push_back(iBottomIdx - (i + 1));
		}

		Mesh* sphereMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Spheremesh", sphereMesh);
		sphereMesh->CreateVertexBuffer(sphereVtx.data(), static_cast<UINT>(sphereVtx.size()));
		sphereMesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

#pragma endregion

	}


	void SetUpState()
	{

#pragma region InputLayout
		D3D11_INPUT_ELEMENT_DESC arrLayout[6] = {}; 

		arrLayout[0].AlignedByteOffset = 0; 
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; 
		arrLayout[0].InputSlot = 0; 
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 16; 
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; 
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 32;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;

		arrLayout[3].AlignedByteOffset = 40;
		arrLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[3].InputSlot = 0;
		arrLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[3].SemanticName = "TANGENT";
		arrLayout[3].SemanticIndex = 0;

		arrLayout[4].AlignedByteOffset = 52;
		arrLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[4].InputSlot = 0;
		arrLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[4].SemanticName = "BINORMAL";
		arrLayout[4].SemanticIndex = 0;

		arrLayout[5].AlignedByteOffset = 64;
		arrLayout[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[5].InputSlot = 0;
		arrLayout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[5].SemanticName = "NORMAL";
		arrLayout[5].SemanticIndex = 0;

		//Vector3 tangent;
		//Vector3 biNormal;
		//Vector3 normal;

		Shader* Meshshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"MeshShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Meshshader->GetVSBlobBufferPointer()
			, Meshshader->GetVSBlobBufferSize()
			, Meshshader->GetInputLayoutAddr());


		Shader* Spriteshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SpriteShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Spriteshader->GetVSBlobBufferPointer()
			, Spriteshader->GetVSBlobBufferSize()
			, Spriteshader->GetInputLayoutAddr());


		Shader* UIshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UIShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, UIshader->GetVSBlobBufferPointer()
			, UIshader->GetVSBlobBufferSize()
			, UIshader->GetInputLayoutAddr());

		Shader* Fadeshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FadeShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Fadeshader->GetVSBlobBufferPointer()
			, Fadeshader->GetVSBlobBufferSize()
			, Fadeshader->GetInputLayoutAddr());

		Shader* Colorshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ColorShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Colorshader->GetVSBlobBufferPointer()
			, Colorshader->GetVSBlobBufferSize()
			, Colorshader->GetInputLayoutAddr());

		Shader* Gridshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"GridShader");
		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Gridshader->GetVSBlobBufferPointer()
			, Gridshader->GetVSBlobBufferSize()
			, Gridshader->GetInputLayoutAddr());

		Shader* Debugshader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugShader");

		dru::GetDevice()->CreateInputLayout(arrLayout, 3
			, Debugshader->GetVSBlobBufferPointer()
			, Debugshader->GetVSBlobBufferSize()
			, Debugshader->GetInputLayoutAddr());

		Shader* particleShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ParticleShader");
		GetDevice()->CreateInputLayout(arrLayout, 3
			, particleShader->GetVSBlobBufferPointer()
			, particleShader->GetVSBlobBufferSize()
			, particleShader->GetInputLayoutAddr());

		Shader* postProcessShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PostProcessShader");
		GetDevice()->CreateInputLayout(arrLayout, 3
			, postProcessShader->GetVSBlobBufferPointer()
			, postProcessShader->GetVSBlobBufferSize()
			, postProcessShader->GetInputLayoutAddr());

		Shader* debugGeometryShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugGeometryShader");
		GetDevice()->CreateInputLayout(arrLayout, 3
			, debugGeometryShader->GetVSBlobBufferPointer()
			, debugGeometryShader->GetVSBlobBufferSize()
			, debugGeometryShader->GetInputLayoutAddr());

		Shader* phongShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PhongShader");
		GetDevice()->CreateInputLayout(arrLayout, 6
			, phongShader->GetVSBlobBufferPointer()
			, phongShader->GetVSBlobBufferSize()
			, phongShader->GetInputLayoutAddr());

		Shader* flatShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FlatShader");
		GetDevice()->CreateInputLayout(arrLayout, 6
			, flatShader->GetVSBlobBufferPointer()
			, flatShader->GetVSBlobBufferSize()
			, flatShader->GetInputLayoutAddr());

#pragma endregion

#pragma region SamplerState
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Point)].GetAddressOf());
		samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Anisotropic)].GetAddressOf());

		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Point), 1, samplerState[static_cast<UINT>(eSamplerType::Point)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Linear), 1, samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Anisotropic), 1, samplerState[static_cast<UINT>(eSamplerType::Anisotropic)].GetAddressOf());

#pragma endregion

#pragma region RasterizerState
		D3D11_RASTERIZER_DESC  reDesc = {};

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(dru::eRasterizerType::SolidBack)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(dru::eRasterizerType::SolidFront)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(dru::eRasterizerType::SolidNone)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(dru::eRasterizerType::WireframeNone)].GetAddressOf());

#pragma endregion

#pragma region DepthStencilState

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(dru::eDepthStencilType::Less)].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(dru::eDepthStencilType::Greater)].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(dru::eDepthStencilType::NoWrite)].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(dru::eDepthStencilType::None)].GetAddressOf());

#pragma endregion

#pragma region BlendState

		blendState[static_cast<UINT>(dru::eBlendStateType::Default)] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendState[static_cast<UINT>(dru::eBlendStateType::AlphaBlend)].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendState[static_cast<UINT>(dru::eBlendStateType::OneOne)].GetAddressOf());


#pragma endregion

	}


	void LoadBuffer()
	{
		
		constantBuffers[static_cast<UINT>(eCBType::Transform)] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[static_cast<UINT>(eCBType::Transform)]->Create(sizeof(TransformCB));

		constantBuffers[static_cast<UINT>(eCBType::Material)] = new ConstantBuffer(eCBType::Material);
		constantBuffers[static_cast<UINT>(eCBType::Material)]->Create(sizeof(MaterialCB));

		constantBuffers[static_cast<UINT>(eCBType::Grid)] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[static_cast<UINT>(eCBType::Grid)]->Create(sizeof(GridCB));

		constantBuffers[static_cast<UINT>(eCBType::Color)] = new ConstantBuffer(eCBType::Color);
		constantBuffers[static_cast<UINT>(eCBType::Color)]->Create(sizeof(ColorCB));

		constantBuffers[static_cast<UINT>(eCBType::Animation)] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[static_cast<UINT>(eCBType::Animation)]->Create(sizeof(AnimationCB));

		constantBuffers[static_cast<UINT>(eCBType::Light)] = new ConstantBuffer(eCBType::Light);
		constantBuffers[static_cast<UINT>(eCBType::Light)]->Create(sizeof(LightCB));

		constantBuffers[static_cast<UINT>(eCBType::ParticleSystem)] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[static_cast<UINT>(eCBType::ParticleSystem)]->Create(sizeof(ParticleSystemCB));

		constantBuffers[static_cast<UINT>(eCBType::Noise)] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[static_cast<UINT>(eCBType::Noise)]->Create(sizeof(NoiseCB));

		constantBuffers[static_cast<UINT>(eCBType::PostProcess)] = new ConstantBuffer(eCBType::PostProcess);
		constantBuffers[static_cast<UINT>(eCBType::PostProcess)]->Create(sizeof(PostProcessCB));

		constantBuffers[static_cast<UINT>(eCBType::LaserHit)] = new ConstantBuffer(eCBType::LaserHit);
		constantBuffers[static_cast<UINT>(eCBType::LaserHit)]->Create(sizeof(LaserHitCB));

		
		lightBuffer = new StructedBuffer();
		lightBuffer->Create(sizeof(LightAttribute), 128, eSRVType::SRV, nullptr, true);
	}

	void LoadShader()
	{
		Shader* MeshShader = new Shader();
		MeshShader->Create(dru::eShaderStage::VS, L"PhongVS.hlsl", "main");
		MeshShader->Create(dru::eShaderStage::PS, L"PhongPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"MeshShader", MeshShader);

		Shader* debugGeometryShader = new Shader();
		debugGeometryShader->Create(dru::eShaderStage::VS, L"DebugGeometryVS.hlsl", "main");
		debugGeometryShader->Create(dru::eShaderStage::PS, L"DebugGeometryPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"DebugGeometryShader", MeshShader);

		Shader* phongShader = new Shader();
		phongShader->Create(eShaderStage::VS, L"PhongVS.hlsl", "main");
		phongShader->Create(eShaderStage::PS, L"PhongPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"PhongShader", phongShader);

		Shader* flatShader = new Shader();
		flatShader->Create(eShaderStage::VS, L"FlatVS.hlsl", "main");
		flatShader->Create(eShaderStage::PS, L"FlatPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"FlatShader", flatShader);


		Shader* SpriteShader = new Shader();
		SpriteShader->Create(dru::eShaderStage::VS, L"SpriteVS.hlsl", "main");
		SpriteShader->Create(dru::eShaderStage::PS, L"SpritePS.hlsl", "main");		
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"SpriteShader", SpriteShader);

		Shader* GridShader = new Shader();
		GridShader->Create(dru::eShaderStage::VS, L"GridVS.hlsl", "main");
		GridShader->Create(dru::eShaderStage::PS, L"GridPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"GridShader", GridShader);

		Shader* UIShader = new Shader();
		UIShader->Create(dru::eShaderStage::VS, L"SpriteVS.hlsl", "main");
		UIShader->Create(dru::eShaderStage::PS, L"UIPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"UIShader", UIShader);

		Shader* FadeShader = new Shader();
		FadeShader->Create(dru::eShaderStage::VS, L"SpriteVS.hlsl", "main");
		FadeShader->Create(dru::eShaderStage::PS, L"FadePS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"FadeShader", FadeShader);

		Shader* ColorShader = new Shader();
		ColorShader->Create(dru::eShaderStage::VS, L"SpriteVS.hlsl", "main");
		ColorShader->Create(dru::eShaderStage::PS, L"ColorPS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"ColorShader", ColorShader);

		Shader* DebugShader = new Shader();
		DebugShader->Create(dru::eShaderStage::VS, L"DebugVS.hlsl", "main");
		DebugShader->Create(dru::eShaderStage::PS, L"DebugPS.hlsl", "main");
		DebugShader->SetRSState(eRasterizerType::SolidNone);
		DebugShader->SetDSState(eDepthStencilType::NoWrite);
		DebugShader->SetBSState(eBlendStateType::AlphaBlend);
		DebugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"DebugShader", DebugShader);

		PaintShader* paintShader = new PaintShader();
		paintShader->Create(L"PaintCS.hlsl", "main");
		GETSINGLE(ResourceMgr)->Insert<PaintShader>(L"PaintShader", paintShader);

		{
			Shader* particleShader = new Shader();
			particleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
			particleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
			particleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
			particleShader->SetRSState(eRasterizerType::SolidNone);
			particleShader->SetDSState(eDepthStencilType::NoWrite);
			particleShader->SetBSState(eBlendStateType::AlphaBlend);
			particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"ParticleShader", particleShader);
		}


		ParticleShader* particleCS = new ParticleShader();
		GETSINGLE(ResourceMgr)->Insert<ParticleShader>(L"ParticleCS", particleCS);
		particleCS->Create(L"ParticleCS.hlsl", "main");

		Shader* postProcessShader = new Shader();
		postProcessShader->Create(eShaderStage::VS, L"PostProcessVS.hlsl", "main");
		postProcessShader->Create(eShaderStage::PS, L"PostProcessPS.hlsl", "main");
		postProcessShader->SetDSState(eDepthStencilType::NoWrite);
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"PostProcessShader", postProcessShader);


	}

	void LoadTexture()
	{		
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise1", L"noise/noise_01.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise2", L"noise/noise_02.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise3", L"noise/noise_03.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"default", L"default.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"texCursor", L"MainScene/Cursor.png");

		Texture* uavTexture = new Texture();
		uavTexture->Create(1024, 1024,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS);
		GETSINGLE(ResourceMgr)->Insert<Texture>(L"PaintTexture", uavTexture);

		postProcessTexture =  new Texture();
		postProcessTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
		GETSINGLE(ResourceMgr)->Insert<Texture>(L"PostProcessTexture", postProcessTexture);
	}


	void LoadMaterial()
	{

		Texture* Meshtexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
		Shader* MeshShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"MeshShader");
		Material* MeshMaterial = new Material();
		MeshMaterial->SetRenderingMode(eRenderingMode::Transparent);
		MeshMaterial->SetShader(MeshShader);
		MeshMaterial->SetTexture(Meshtexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"MeshMaterial", MeshMaterial);

		Texture* Spritetexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
		Shader* SpriteShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SpriteShader");
		Material* SpriteMaterial = new Material();
		SpriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		SpriteMaterial->SetShader(SpriteShader);
		SpriteMaterial->SetTexture(Spritetexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"SpriteMaterial", SpriteMaterial);

		Texture* UItexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Title");
		Shader* UIShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UIShader");
		Material* UIMaterial = new Material();
		UIMaterial->SetRenderingMode(eRenderingMode::Transparent);
		UIMaterial->SetShader(UIShader);
		UIMaterial->SetTexture(UItexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"UIMaterial", UIMaterial);

		Shader* GridShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"GridShader");
		Material* GridMaterial = new Material();
		GridMaterial->SetRenderingMode(eRenderingMode::Opaque);
		GridMaterial->SetShader(GridShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"GridMaterial", GridMaterial);

		Texture* Fadetexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
		Shader* FadeShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FadeShader");
		Material* FadeMaterial = new Material();
		FadeMaterial->SetRenderingMode(eRenderingMode::Transparent);
		FadeMaterial->SetShader(FadeShader);
		FadeMaterial->SetTexture(Fadetexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"FadeMaterial", FadeMaterial);

		Texture* Colortexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Black");
		Shader* ColorShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ColorShader");
		Material* ColorMaterial = new Material();
		ColorMaterial->SetRenderingMode(eRenderingMode::Transparent);
		ColorMaterial->SetShader(ColorShader);
		ColorMaterial->SetTexture(Colortexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"ColorMaterial", ColorMaterial);

		Texture* Painttexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"PaintTexture");
		Shader* PaintShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"MeshShader");
		Material* PaintMaterial = new Material();

		PaintMaterial->SetShader(PaintShader);
		PaintMaterial->SetTexture(Painttexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"PaintMaterial", PaintMaterial);

		Shader* particleShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ParticleShader");
		Material* particleMaterial = new Material();
		particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		particleMaterial->SetShader(particleShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"ParticleMaterial", particleMaterial);

		Shader* DebugShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugShader");
		Material* DebugMaterial = new Material();
		DebugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		DebugMaterial->SetShader(DebugShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"DebugMaterial", DebugMaterial);

		Shader* postProcessShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PostProcessShader");
		Material* postProcessMaterial = new Material();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		postProcessMaterial->SetShader(postProcessShader);
		postProcessMaterial->SetTexture(postProcessTexture);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"PostProcessMaterial", postProcessMaterial);

		Shader* debugGeometryShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugGeometryShader");
		Material* debugGeometryMaterial = new Material();
		debugGeometryMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugGeometryMaterial->SetShader(debugGeometryShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"DebugGeometryMaterial", debugGeometryMaterial);

		Shader* phongShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PhongShader");
		Material* phongMaterial = new Material();
		phongMaterial->SetRenderingMode(eRenderingMode::Transparent);
		phongMaterial->SetShader(phongShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"PhongMaterial", phongMaterial);

		Shader* flatShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FlatShader");
		Material* flatMaterial = new Material();
		flatMaterial->SetRenderingMode(eRenderingMode::Transparent);
		flatMaterial->SetShader(flatShader);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"FlatMaterial", flatMaterial);
			
			
		{
			Material* material = new Material(L"texCursor", L"UIShader");
			GETSINGLE(ResourceMgr)->Insert<Material>(L"CursorMat", material);
		};
	}




	void Initialize()
	{
		
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}

	void release()
	{
		for (size_t i = 0; i < static_cast<UINT>(eCBType::End); i++)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
		delete lightBuffer;
		lightBuffer = nullptr;
	}

	void Render()
	{
		GetDevice()->OMSetRenderTarget();

		BindNoiseTexture();
		BindLight();

		UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());

		for (Camera* cam : Cameras[type])
		{
			if (nullptr == cam)
				continue;

			cam->Render();
		}
		Cameras[type].clear();
		renderer::lights.clear();
	}

	void PushLightAttribute(LightAttribute _attribute)
	{
		lights.push_back(_attribute);
	}

	void BindLight()
	{
		lightBuffer->SetData(lights.data(), static_cast<UINT>(lights.size()));
		lightBuffer->BindSRV(eShaderStage::VS, 13);
		lightBuffer->BindSRV(eShaderStage::PS, 13);

		renderer::LightCB Lightcb = {};
		Lightcb.lightCount = static_cast<UINT>(lights.size());

		ConstantBuffer* cb = constantBuffers[static_cast<UINT>(eCBType::Light)];
		cb->SetData(&Lightcb);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	float noiseTime = 10.f;
	float ElapsedTime = 0.f;

	void BindNoiseTexture()
	{
		Texture* noise = GETSINGLE(ResourceMgr)->Find<Texture>(L"noise1");
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);

		NoiseCB info = {};
		info.noiseSize.x = static_cast<float>(noise->GetWidth());
		info.noiseSize.y = static_cast<float>(noise->GetHeight());
		noiseTime -= DT;
		info.noiseTime = noiseTime;

		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Noise)];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}
	void CopyRenderTarget()
	{
		Texture* renderTarget = GETSINGLE(ResourceMgr)->Find<Texture>(L"RenderTargetTexture");

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = postProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GetDevice()->CopyResource(dest, source);

		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}
}
