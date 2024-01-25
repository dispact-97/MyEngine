#include "BoundingBox.h"

BoundingBox::BoundingBox(RenderableBase* pObject)
	: m_min(),
	m_max()
{

}

BoundingBox::~BoundingBox()
{

}

void BoundingBox::SetBoundingBox(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
	m_min = min;
	m_max = max;
}

const DirectX::XMFLOAT3 BoundingBox::GetBoundingMin()
{
	return m_min;
}

const DirectX::XMFLOAT3 BoundingBox::GetBoundingMax()
{
	return m_max;
}


