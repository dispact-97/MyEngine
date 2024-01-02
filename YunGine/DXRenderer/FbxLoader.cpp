#include "FbxLoader.h"
#include "RenderableBase.h"

FbxLoader::FbxLoader(const std::string& filename)
	: m_pScene(nullptr),
	m_Meshes(),
	m_numVertices(0),
	m_numMaterial(0),
	m_pMaterial(nullptr)
{

}

FbxLoader::FbxLoader()
	: m_pScene(nullptr),
	m_Meshes(),
	m_numVertices(0),
	m_numMaterial(0),
	m_pMaterial(nullptr)
{

}

FbxLoader::~FbxLoader()
{

}

void FbxLoader::InitScene()
{

}

void FbxLoader::InitMesh(UINT index, const aiMesh* pMesh)
{

}

void FbxLoader::SetMeshes(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext)
{

}

bool FbxLoader::LoadModel(const std::string& pFile,RenderableBase* outPtr)
{
	Assimp::Importer importer;
	m_pScene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
		return false;
	}

	if (m_pScene)
	{
		//m_Meshes = m_pScene->mNumMeshes;
		m_numMaterial = m_pScene->mNumMaterials;
	}

	importer.FreeScene();

	return true;
}

//bool FbxLoader::LoadModel(const std::string& pFile)
//{
//	Assimp::Importer importer;
//
//	const aiScene* scene = importer.ReadFile(
//		pFile,
//		aiProcess_CalcTangentSpace |
//		aiProcess_Triangulate |
//		aiProcess_JoinIdenticalVertices |
//		aiProcess_SortByPType);
//
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
//	{
//		return false;
//	}
//
//	scene->mMeshes;
//	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
//	{
//		auto mesh = scene->mMeshes[i];
//		if (mesh->HasFaces())
//		{
//			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
//			{
//				DirectX::XMFLOAT3 vertex = { mesh->mVertices->x,mesh->mVertices->y,mesh->mVertices->z };
//			}
//		}
//
//	}
//		return true;
//}



