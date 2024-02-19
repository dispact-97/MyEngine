#pragma once
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <wrl/client.h>
#include <windows.h>
#include "TextureLoader.h"


class TextureClass final
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	HRESULT Initalize(Microsoft::WRL::ComPtr<ID3D11Device> device, WCHAR* file);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* _TextureResource;
};

#endif