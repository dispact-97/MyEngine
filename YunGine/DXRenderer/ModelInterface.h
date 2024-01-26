#pragma once
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <d3d11.h>

#include "Vertex.h"

class ModelInterface
{
public:

	virtual void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) abstract;
	virtual void Render() abstract;

};