#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <DirectXMath.h>

#include <windows.h>
#include <vector>

struct vertexDatas
{
	DirectX::XMFLOAT3 m_pos = {};
	DirectX::XMFLOAT3 m_normal = {};
	DirectX::XMFLOAT2 m_tex = {};

	vertexDatas() {}
	vertexDatas(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT2& tex) :
		m_pos(pos),m_normal(normal),m_tex(tex) {}
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

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	bool DoTheImortThing(const std::string& pFile);
private:




};