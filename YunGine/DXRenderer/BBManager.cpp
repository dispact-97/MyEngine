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

bool BBManager::checkBoundingBox(NewCube* cube, DirectX::BoundingFrustum& _CameraFrustum)
{
	if (!cube->_objectBoundingBox)
	{
		return false;
	}

	return _CameraFrustum.Contains(*cube->_objectBoundingBox) != DirectX::DISJOINT;
}
