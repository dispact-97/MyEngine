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
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		return false;
	}

	scene->mMeshes;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		if (mesh->HasFaces())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				DirectX::XMFLOAT3 vertex = { mesh->mVertices->x,mesh->mVertices->y,mesh->mVertices->z };
			}
		}

	}
		return true;
}

