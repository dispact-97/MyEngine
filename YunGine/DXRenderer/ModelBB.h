#pragma once

#include <DirectXCollision.h>
#include "RenderableBase.h"

class ModelInterface;

// Test
class NewCube;

class ModelBB
{
public:
	ModelBB();
	~ModelBB();

	HRESULT SetBoundingBox(ModelInterface* object);

	HRESULT SetBoundingBox(NewCube* cube);

	DirectX::BoundingBox* _objectBoundingBox;

	bool IsBoxInViewFrustum(const DirectX::BoundingBox* boundingBox, const DirectX::XMMATRIX& viewProjMatrix);

private:
	HRESULT CreateBoundingBox(DirectX::BoundingBox*, const std::vector<DirectX::XMFLOAT3>&);

};