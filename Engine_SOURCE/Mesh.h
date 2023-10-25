#pragma once
#include "EngineResource.h"
#include "Graphics.h"
#include "SimpleMath.h"

class Mesh : public Resource
{
public:
	Mesh();
	virtual ~Mesh();

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;

	bool CreateVertexBuffer(void* data, UINT count);
	bool CreateInstanceBuffer(void* data, UINT count);
	bool CreateIndexBuffer(void* data, UINT count);

	void BindBuffer(bool drawInstance = false);

	void Render();

	void RenderInstanced(UINT count);

	bool GetVerticesFromBuffer(std::vector<Vertex>* vertexVec);
	bool GetIndexesFromBuffer(std::vector<UINT>* indexVec);

	void UpdateInstanceBuffer(std::vector<InstancingData> matrices);

	void CheckFrustumCull();

public:
	bool IsRender() { return mbRender; }
	void SetRender(bool render) { mbRender = render; }

	UINT GetVertexCount() { return mVertexCount; }
	UINT GetIndexCount() { return mIndexCount; }

	void SetMinVertex(const math::Vector3 & vertex);
	void SetMaxVertex(const math::Vector3 & vertex);

	void SetWorldMatrix(const math::Matrix& worldMatrix) { mOwnerWorldMatrix = worldMatrix; }

	GETSET(bool, mbFrustumCulled, FrustumCull)

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	D3D11_BUFFER_DESC mVBDesc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	D3D11_BUFFER_DESC mIBDesc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mInstancedBuffer;
	D3D11_BUFFER_DESC mISTBDesc;

	UINT			mVertexCount;
	UINT			mIndexCount;
	bool			mbRender;
	bool			mbFrustumCulled;

	// aabb
	BoundingBox mBoundingBox; // 오류가 생기면 CreateFromPoints이걸로 만들어보자
	math::Vector3 mMinVertex;
	math::Vector3 mMaxVertex;

	math::Vector3 mInitialExtent;

	math::Matrix mOwnerWorldMatrix;

//public:
//	// debug용
//	static UINT sMeshCount;
//	bool r;
};
