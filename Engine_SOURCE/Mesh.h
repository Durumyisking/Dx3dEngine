#pragma once
#include "EngineResource.h"
#include "Graphics.h"

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


//		Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer(eBufferStage _eStage);

private:
	// �޽� �׸����� �������� �ε��� ������ ������ ��
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	D3D11_BUFFER_DESC mVBDesc;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	D3D11_BUFFER_DESC mIBDesc;


	UINT			mIndexCount;

};
