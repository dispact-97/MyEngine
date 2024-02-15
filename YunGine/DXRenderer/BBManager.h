#pragma once

#include <SimpleMath.h>

class ModelInterface;
class ModelBB;
class DirectX::BoundingFrustum;

// Test
class NewCube;


class BBManager
{
public:

	static BBManager* GetInstance()
	{
		if (!SingleTonBBManager)
		{
			SingleTonBBManager = new BBManager();
		}
		return SingleTonBBManager;
	}

	BBManager(const BBManager&) = delete;
	BBManager& operator=(const BBManager&) = delete;

	~BBManager();

	bool checkBoundingBox(NewCube* cube, DirectX::BoundingFrustum& _CameraFrustum);

private:
	BBManager();
	static BBManager* SingleTonBBManager;

	std::vector<ModelInterface*> _BBVector;

};