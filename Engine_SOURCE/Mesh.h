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
	bool CreateInstanceBuffer(void* worldMatrix);
	bool CreateIndexBuffer(void* data, UINT count);

	void BindBuffer();

	void Render();

	void RenderInstanced(UINT count);

	bool GetVerticesFromBuffer(std::vector<Vertex>* vertexVec);
	bool GetIndexesFromBuffer(std::vector<UINT>* indexVec);

public:
	bool IsRender() { return mbRender; }
	void SetRender(bool render) { mbRender = render; }

	//GETSET(UINT, mVertexCount, VertexCount);
	//GETSET(UINT, mIndexCount, IndexCount);

	void SetVertexCount(UINT count) { mVertexCount = count; }
	void SetIndexCount(UINT count) { mIndexCount = count; }
	UINT GetVertexCount() { return mVertexCount; }
	UINT GetIndexCount() { return mIndexCount; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	D3D11_BUFFER_DESC mVBDesc;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mInstancedBuffer;
	D3D11_BUFFER_DESC mISTBDesc;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	D3D11_BUFFER_DESC mIBDesc;

	UINT			mVertexCount;
	UINT			mIndexCount;
	bool			mbRender;
};
