#include "InstantiativeObject.h"
#include "InstancingContainer.h"

InstantiativeObject::InstantiativeObject()
	:mContainer{}
	, mInstanceID(0)
{
}

InstantiativeObject::InstantiativeObject(const InstantiativeObject& Obj)
	: DynamicObject(Obj)
{
}

InstantiativeObject::~InstantiativeObject()
{
}

InstantiativeObject* InstantiativeObject::Clone() const
{
	return new InstantiativeObject(*this);
}

void InstantiativeObject::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void InstantiativeObject::Load(FILE* File)
{
	DynamicObject::Load(File);
}

void InstantiativeObject::Initialize()
{
	DynamicObject::Initialize();
}

void InstantiativeObject::Update()
{
	DynamicObject::Update();
}

void InstantiativeObject::FixedUpdate()
{
	DynamicObject::FixedUpdate();


	if (mContainer)
	{
		InstancingData data
		{
			GetTransform()->GetWorldMatrix(),
			GetTransform()->GetWorldMatrix().Invert().Transpose()
		};

		mContainer->SetObjectInstancingData(mInstanceID, data);
	}
}

void InstantiativeObject::Render()	
{
	if (!mContainer)
	{
		DynamicObject::Render();
	}
}

void InstantiativeObject::PrevRender()
{
	if (!mContainer)
	{
		DynamicObject::PrevRender();
	}
}

void InstantiativeObject::FontRender()
{
	DynamicObject::FontRender();
}
