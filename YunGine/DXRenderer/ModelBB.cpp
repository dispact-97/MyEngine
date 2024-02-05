#include "ModelBB.h"
#include "ModelInterface.h"

#include "NewCube.h"

ModelBB::ModelBB()
	: m_min(),
	m_max(),
	_objectBoundingBox(),
	_frustum()
{

}

ModelBB::~ModelBB()
{

}

HRESULT ModelBB::SetBoundingBox(ModelInterface* object)
{
	HRESULT hr = S_OK;


	return hr;
}

HRESULT ModelBB::SetBoundingBox(NewCube* cube)
{
	HRESULT hr = S_OK;

	DirectX::BoundingBox* tempBoundingBox;
	
	//cube->_objectBoundingBox = tempBoundingBox;

	return hr;
}

bool ModelBB::IsBoxInViewFrustum(const DirectX::BoundingBox* outBoundingBox, const DirectX::XMMATRIX& viewProjMatrix)
{
	DirectX::BoundingFrustum tempFrustum;
	DirectX::BoundingFrustum::CreateFromMatrix(tempFrustum, viewProjMatrix);

	return true;
}
