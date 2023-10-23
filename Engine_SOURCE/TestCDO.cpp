#include "TestCDO.h"

TestCDO::TestCDO()
{
}

TestCDO::TestCDO(const TestCDO& Obj)
    : GameObj(Obj)
{
}

TestCDO::~TestCDO()
{
}

TestCDO* TestCDO::Clone() const
{
    return nullptr;
}

void TestCDO::Save(FILE* File)
{
    GameObj::Save(File);
}

void TestCDO::Load(FILE* File)
{
    GameObj::Load(File);
}

void TestCDO::Initialize()
{
    GameObj::Initialize();
}

void TestCDO::Update()
{
    GameObj::Update();
}

void TestCDO::FixedUpdate()
{
    GameObj::FixedUpdate();
}

void TestCDO::Render()
{
    GameObj::Render();
}
