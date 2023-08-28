#pragma once
#include "Engine.h"

class ID3D11Buffer;
class Skybox
{
public:
	Skybox();
	~Skybox();
	void Draw();
private:
	ID3D11Buffer* vertex_buffer_ptr = nullptr;
	UINT vertex_stride = 0;
	UINT vertex_offset = 0;
};