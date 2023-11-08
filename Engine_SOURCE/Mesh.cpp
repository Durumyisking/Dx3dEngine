#include "Mesh.h"
#include "Renderer.h"
#include "GraphicDevice.h"

//UINT Mesh::sMeshCount = 0;

Mesh::Mesh()
	: Resource(eResourceType::Mesh)
	, mVBDesc{}
	, mIBDesc{}
	, mISTBDesc{}
	, mVertexCount(0)
	, mIndexCount(0)
	, mbRender(true)
	, mBoundingBox{}
	, mMinVertex{ INFINITY }
	, mMaxVertex{ -INFINITY }
	, mbFrustumCulled(false)
	, mOwnerWorldMatrix{}
	, mInitialExtent{}
	//, r (true)
{
}
Mesh::~Mesh()
{
}
HRESULT Mesh::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}
HRESULT Mesh::LoadFullpath(const std::wstring& path)
{
	return E_NOTIMPL;
}
bool Mesh::CreateVertexBuffer(void* data, UINT count)
{
	//++sMeshCount;
	mVertexCount = count;
	mVBDesc.ByteWidth = sizeof(Vertex) * count;
	mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	mVBDesc.CPUAccessFlags = 0; // 상수버퍼 통해서 값 변경할꺼임

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = data;

	if (!GetDevice()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()))
		return false;

	mBoundingBox.Center = (mMinVertex + mMaxVertex) * 0.5f;
	mBoundingBox.Extents = (mMaxVertex - mMinVertex) * 0.5f;
	mInitialExtent = mBoundingBox.Extents;
	return true;
}

bool Mesh::CreateInstanceBuffer(void* data, UINT count)
{
	ZeroMemory(&mISTBDesc, sizeof(mISTBDesc));
	mISTBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	mISTBDesc.ByteWidth = sizeof(InstancingData) * count;
	mISTBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	mISTBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//mISTBDesc.MiscFlags = 0;
	//mISTBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = data;

	if (!GetDevice()->CreateBuffer(&mISTBDesc, &subData, mInstancedBuffer.GetAddressOf()))
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


void Mesh::BindBuffer(bool drawInstance)
{
	// ui는 컬링하면 안돼
	if (GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh") != this)
	{
		//CheckFrustumCull();
		//if (mbFrustumCulled)
		//	return;
	}

	if (drawInstance)
	{
		UINT stride[2] = { sizeof(Vertex), sizeof(InstancingData) };
		UINT offset[2] = { 0, 0 };
		ID3D11Buffer* vbs[2] = { mVertexBuffer.Get(), mInstancedBuffer.Get() };
		GetDevice()->BindVertexBuffer(0, 2, vbs, stride, offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	else
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		GetDevice()->BindVertexBuffer(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}


void Mesh::Render()
{
	if (!mbRender || mbFrustumCulled)
		return;


	GetDevice()->DrawIndexed(mIndexCount, 0, 0);
}

void Mesh::RenderInstanced(UINT count)
{
	if (!IsRender())
		return;

	GetDevice()->DrawIndexedInstanced(mIndexCount, count, 0, 0, 0);
}

bool Mesh::GetVerticesFromBuffer(std::vector<Vertex>* vertexVec)
{
	D3D11_BUFFER_DESC desc = {};
	mVertexBuffer->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0; // Remove any bind flags
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	Microsoft::WRL::ComPtr<ID3D11Buffer> stagingBuffer = nullptr;
	GetDevice()->CreateBuffer(&desc, nullptr, &stagingBuffer);

	// Copy data to staging buffer
	GetDevice()->GetDeviceContext()->CopyResource(stagingBuffer.Get(), mVertexBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	vertexVec->clear();
	vertexVec->resize(mVertexCount);

	// Map the buffer
	HRESULT hr = GetDevice()->GetDeviceContext()->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedData);
	if (FAILED(hr))
		return false;

	// Copy the vertices from the buffer to the vector
	Vertex* bufferData = reinterpret_cast<Vertex*>(mappedData.pData);
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		(*vertexVec)[i] = bufferData[i];
	}

	// Unmap the buffer
	GetDevice()->GetDeviceContext()->Unmap(stagingBuffer.Get(), 0);

	return true;
}


bool Mesh::GetIndexesFromBuffer(std::vector<UINT>* indexVec)
{
	D3D11_BUFFER_DESC desc = {};
	mIndexBuffer->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0; // Remove any bind flags
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	Microsoft::WRL::ComPtr<ID3D11Buffer> stagingBuffer = nullptr;
	GetDevice()->CreateBuffer(&desc, nullptr, &stagingBuffer);

	// Copy data to staging buffer
	GetDevice()->GetDeviceContext()->CopyResource(stagingBuffer.Get(), mIndexBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	indexVec->clear();
	indexVec->resize(mIndexCount);

	// Map the buffer
	HRESULT hr = GetDevice()->GetDeviceContext()->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedData);
	if (FAILED(hr))
		return false;

	// Copy the vertices from the buffer to the vector
	UINT* bufferData = reinterpret_cast<UINT*>(mappedData.pData);
	for (UINT i = 0; i < mIndexCount; ++i)
	{
		(*indexVec)[i] = bufferData[i];
	}

	// Unmap the buffer
	GetDevice()->GetDeviceContext()->Unmap(stagingBuffer.Get(), 0);

	return true;
}

void Mesh::UpdateInstanceBuffer(std::vector<InstancingData> matrices)
{
	UINT size = static_cast<UINT>((matrices.capacity() * sizeof(InstancingData)));
	GetDevice()->BindBuffer(mInstancedBuffer.Get(), matrices.data(), size);
}

void Mesh::CheckFrustumCull()
{
	BoundingFrustum frustum = renderer::mainCamera->GetFrustum();
	mBoundingBox.Center = Vector3::Transform(mBoundingBox.Center, mOwnerWorldMatrix);

	Vector3 extractedScale = { mOwnerWorldMatrix._11, mOwnerWorldMatrix._22, mOwnerWorldMatrix._33 };
	mBoundingBox.Extents = mInitialExtent * extractedScale; 
	mbFrustumCulled = !frustum.Intersects(mBoundingBox);
}

void Mesh::SetMinVertex(const math::Vector3& vertex)
{
	mMinVertex = DirectX::XMVectorMin(mMinVertex, vertex);
}

void Mesh::SetMaxVertex(const math::Vector3& vertex)
{
	mMaxVertex = DirectX::XMVectorMax(mMaxVertex, vertex);
}
