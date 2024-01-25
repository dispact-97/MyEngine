#pragma once

#include "RenderableBase.h"

class BoundingBox final : public RenderableBase
{
public:
	BoundingBox(RenderableBase* pObject);
	~BoundingBox();

	void SetBoundingBox(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	const DirectX::XMFLOAT3 GetBoundingMin();
	const DirectX::XMFLOAT3 GetBoundingMax();

private:
	DirectX::XMFLOAT3 m_min;
	DirectX::XMFLOAT3 m_max;


};