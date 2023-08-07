#include "Mesh.h"
#include "Renderer.h"
#include "GraphicDevice.h"



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
bool Mesh::CreateVertexBuffer(void* data, UINT count)
{
	mVBDesc.ByteWidth = sizeof(renderer::Vertex) * count;
	mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	mVBDesc.CPUAccessFlags = 0; // 상수버퍼 통해서 값 변경할꺼임

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = data;

	if (!GetDevice()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()))
		return false;
		
	return true;
}

bool Mesh::CreateIndexBuffer(void* data, UINT count)
{
	mIndexCount = count;
	mIBDesc.ByteWidth = sizeof(UINT) * count;
	mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	mIBDesc.CPUAccessFlags = 0; 

	D3D11_SUBRESOURCE_DATA idxData = {};
	idxData.pSysMem = data;

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

void Mesh::RenderInstanced(UINT count)
{
	GetDevice()->DrawIndexedInstanced(mIndexCount, count, 0, 0, 0);
}
