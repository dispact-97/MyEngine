#include "BBManager.h"
#include "ModelBB.h"

// Test
#include "NewCube.h"
#include "ModelInterface.h"

BBManager::BBManager()
{

}

BBManager::~BBManager()
{

}

void BBManager::addBoxToList(ModelInterface* object)
{
	//object->


}

void BBManager::addBoxToListTest(NewCube* cubePtr)
{
	//cubePtr->_objectBoundingBox;

	//_BBVector.push_back(cubePtr);


}

void BBManager::checkBoundingBox(std::vector<ModelInterface*>& objectList)
{
	DirectX::BoundingFrustum frustm;
	//DirectX::BoundingFrustum::CreateFromMatrix();

		//for (auto& iter : boxList)
		//{
			//iter->IsBoxInViewFrustum();
		//}
}

void BBManager::checkBoundingBox(NewCube* cube, const DirectX::XMMATRIX& viewProjMatrix)
{
	// 나중에 vector에 넣고 돌리기
	cube->IsBoxInViewFrustum(cube->_objectBoundingBox,viewProjMatrix);
}
