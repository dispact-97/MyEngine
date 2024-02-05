#pragma once

#include <vector>
class ModelInterface;
class ModelBB;

// Test
class NewCube;

class BBManager
{
public:
	BBManager();
	~BBManager();

	void addBoxToList(ModelInterface* object);
	void addBoxToListTest(NewCube* cubePtr);

	void checkBoundingBox(std::vector<ModelInterface*>& boundingBoxList);
	void checkBoundingBox(NewCube* cube,const DirectX::XMMATRIX& viewProjMatrix);

private:
	std::vector<ModelInterface*> _BBVector;
};