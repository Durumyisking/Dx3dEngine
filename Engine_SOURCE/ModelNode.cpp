#include "ModelNode.h"

ModelNode::ModelNode()
	: mName(L"")
	, mTransformation{}
	, mRootName(L"")
	, mRootNode(nullptr)
	, mChilds{}

{
}

ModelNode::~ModelNode()
{
}
