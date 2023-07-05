#include "Mesh.h"
#include "Renderer.h"
#include "GraphicDevice.h"

namespace dru
{
	Mesh::Mesh()
		: Resource(eResourceType::Mesh)
		, mVBDesc{}
		, mIBDesc{}
		, mIndexCount(0)
	{
	}
	Mesh::~Mesh()
	{
	}
	HRESULT Mesh::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	bool Mesh::CreateVertexBuffer(void* _Data, UINT _Count)
	{
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * _Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0; // 상수버퍼 통해서 값 변경할꺼임

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _Data;

		if (!GetDevice()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()))
			return false;
		
		return true;
	}

	bool Mesh::CreateIndexBuffer(void* _Data, UINT _Count)
	{
		mIndexCount = _Count;
		mIBDesc.ByteWidth = sizeof(UINT) * _Count;
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mIBDesc.CPUAccessFlags = 0; 

		D3D11_SUBRESOURCE_DATA idxData = {};
		idxData.pSysMem = _Data;

		if (!GetDevice()->CreateBuffer(&mIBDesc, &idxData, mIndexBuffer.GetAddressOf()))
			return false;

		return true;
	}


	void Mesh::BindBuffer()
	{
		UINT stride = sizeof(renderer::Vertex);
		UINT offset = 0;

		GetDevice()->BindVertexBuffer(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}


	void Mesh::Render()
	{
		GetDevice()->DrawIndexed(mIndexCount, 0, 0);
	}

	void Mesh::RenderInstanced(UINT _Count)
	{
		GetDevice()->DrawIndexedInstanced(mIndexCount, _Count, 0, 0, 0);
	}

}