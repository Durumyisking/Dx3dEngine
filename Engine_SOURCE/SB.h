#pragma once
#include "Engine.h"

class Mesh;
class ScreenQuad
{
public:
	ScreenQuad();
	~ScreenQuad();
	void Draw();
};

class Skybox
{
public:
	Skybox();
	~Skybox();
	void Draw();

private:
	Mesh* mMesh;
};