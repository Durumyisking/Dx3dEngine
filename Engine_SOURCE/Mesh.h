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
	bool CreateIndexBuffer(void* data, UINT count);

	void BindBuffer();

	void Render();

	void RenderInstanced(UINT count);

	//GetVerticesFromBuffer<renderer::Vertex>(GetDevice()->GetDeviceContext().Get();)
	template <typename VertexType>
	std::vector<VertexType> GetVerticesFromBuffer(ID3D11DeviceContext* deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		std::vector<VertexType> vertices(mVertexCount);

		// Map the buffer
		HRESULT hr = deviceContext->Map(mVertexBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedData);
		if (SUCCEEDED(hr))
		{
			// Copy the vertices from the buffer to the vector
			VertexType* bufferData = reinterpret_cast<VertexType*>(mappedData.pData);
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				vertices[i] = bufferData[i];
			}

			// Unmap the buffer
			deviceContext->Unmap(mVertexBuffer.Get(), 0);
		}

		return vertices;
	}

public:
	bool IsRender() { return mbRender; }
	void SetRender(bool render) { mbRender = render; }

	//GETSET(UINT, mVertexCount, VertexCount);
	//GETSET(UINT, mIndexCount, IndexCount);

	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() { return mVertexBuffer; }

	void SetVertexCount(UINT count) { mVertexCount = count; }
	void SetIndexCount(UINT count) { mIndexCount = count; }
	UINT GetVertexCount() { return mVertexCount; }
	UINT GetIndexCount() { return mIndexCount; }

	const std::vector<math::Vector4>& GetVertexes() { return mVertexes; }
	void SetVertexes(const std::vector<math::Vector4>& vertexes) { std::copy(vertexes.begin(), vertexes.end(), std::back_inserter(mVertexes)); }

private:
	// 메시 그릴때는 정점정보 인덱스 정보만 있으면 됨
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	D3D11_BUFFER_DESC mVBDesc;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	D3D11_BUFFER_DESC mIBDesc;


	std::vector<math::Vector4> mVertexes;
	UINT			mVertexCount;
	UINT			mIndexCount;
	bool			mbRender;
};
