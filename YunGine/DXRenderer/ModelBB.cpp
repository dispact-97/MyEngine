#include <vector>
#include <algorithm>

#include "ModelBB.h"
#include "ModelInterface.h"

#include "NewCube.h"

ModelBB::ModelBB()
	: m_min(),
	m_max(),
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
	const std::vector<DirectX::XMFLOAT3>& vertices = cube->GetLocalSpaceVertices();
	hr = CreateBoundingBox(cube->_objectBoundingBox, vertices);
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

	SetBoudingBoxScale(*boundingBoxPtr, scale);

	return hr;
}

void ModelBB::SetBoudingBoxScale(DirectX::BoundingBox& boundingBoxRef, float boxScale)
{
	DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&boundingBoxRef.Center);
	DirectX::XMVECTOR extent = DirectX::XMLoadFloat3(&boundingBoxRef.Extents);
	
	center = DirectX::XMVectorScale(center, boxScale); // 중심을 스케일링함
	extent = DirectX::XMVectorScale(extent, boxScale); // 확장을 스케일링함

	DirectX::XMStoreFloat3(&boundingBoxRef.Center, center);
	DirectX::XMStoreFloat3(&boundingBoxRef.Extents, extent);
}
