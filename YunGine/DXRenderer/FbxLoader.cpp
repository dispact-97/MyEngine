#include "FbxLoader.h"

FbxLoader::FbxLoader()
{

}

FbxLoader::~FbxLoader()
{

}

bool FbxLoader::DoTheImortThing(const std::string& pFile)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		pFile,
		aiProcess_CalcTangentSpace		|
		aiProcess_Triangulate			|
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene == nullptr)
	{
		return false;
	}

	return true;

}

