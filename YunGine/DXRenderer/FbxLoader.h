#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <DirectXMath.h>
#include <vector>
#include <windows.h>
#include <string>
struct vertexDatas
{
	DirectX::XMFLOAT3 m_pos = {};
	DirectX::XMFLOAT3 m_normal = {};
	DirectX::XMFLOAT2 m_tex = {};

	vertexDatas() {}
	vertexDatas(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT2& tex) :
		m_pos(pos), m_normal(normal), m_tex(tex) {}
};

struct Mesh
{
	std::vector<vertexDatas>	m_vertices;
	std::vector<int>			m_indices;
	UINT						m_materialIndex;
	Mesh()
	{
		m_materialIndex = 0;
	}
};

class ID3D11Device;
class ID3D11DeviceContext;
class RenderableBase;

class FbxLoader
{
public:
	FbxLoader();
	FbxLoader(const std::string& filename);
	~FbxLoader();

	void InitScene();
	void InitMesh(UINT index, const aiMesh* pMesh);
	void SetMeshes(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	bool LoadModel(const std::string& pFile,RenderableBase* outPtr);

private:
	const aiScene*						m_pScene;	// 모델 정보
	std::vector<Mesh>					m_Meshes;	// 매쉬 정보
	//std::vector<pair<string,Bone>>	m_Bones;	// 본 정보
	aiMaterial*							m_pMaterial;
	UINT								m_numVertices;
	UINT								m_numMaterial;



};