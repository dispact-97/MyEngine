#include <vector>
#include <algorithm>

#include "ModelBB.h"
#include "ModelInterface.h"

#include "NewCube.h"

ModelBB::ModelBB()
	: _objectBoundingBox()
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

HRESULT ModelBB::SetBoundingBox(NewCube* pCube)
{
	HRESULT hr = S_OK;
	const std::vector<DirectX::XMFLOAT3>& vertices = pCube->GetLocalSpaceVertices();
	hr = CreateBoundingBox(pCube->_objectBoundingBox, vertices);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

bool ModelBB::IsBoxInViewFrustum(const DirectX::BoundingBox* outBoundingBox, const DirectX::XMMATRIX& viewProjMatrix)
{
	DirectX::BoundingFrustum tempFrustum;
	DirectX::BoundingFrustum::CreateFromMatrix(tempFrustum, viewProjMatrix);

	return tempFrustum.Contains(*outBoundingBox) != DirectX::DISJOINT;
}

HRESULT ModelBB::CreateBoundingBox(DirectX::BoundingBox* boundingBoxPtr, const std::vector<DirectX::XMFLOAT3>& vertices)
{
	HRESULT hr = S_OK;

	if (vertices.empty())
	{
		return E_INVALIDARG;
	}
	else if (!boundingBoxPtr)
	{
		return E_POINTER;
	}
	else
	{
		DirectX::BoundingBox tempBox;
		//std::vector<DirectX::XMFLOAT3> pointsArray(vertices.begin(), vertices.end());

		//DirectX::BoundingBox::CreateFromPoints(tempBox, pointsArray.size(), pointsArray.data(), sizeof(DirectX::XMFLOAT3));

		DirectX::BoundingBox::CreateFromPoints(tempBox, vertices.size(), vertices.data(), sizeof(DirectX::XMFLOAT3));

		*boundingBoxPtr = tempBox;
	}

	return hr;
}