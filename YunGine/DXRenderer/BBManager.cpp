#include <DirectXCollision.h>

#include "BBManager.h"
#include "ModelBB.h"

// Test
#include "NewCube.h"
#include "ModelInterface.h"

BBManager* BBManager::SingleTonBBManager = nullptr;

BBManager::BBManager()
{

}

BBManager::~BBManager()
{

}

void BBManager::checkBoundingBox(NewCube* cube, DirectX::BoundingFrustum& _CameraFrustum)
{
	DirectX::BoundingFrustum tempFrustum;

	DirectX::BoundingFrustum::CreateFromMatrix(_CameraFrustum, );

	return _CameraFrustum.Contains(*cube->_objectBoundingBox) != DirectX::DISJOINT;
}
