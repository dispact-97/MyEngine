#pragma once
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <wrl/client.h>
#include <windows.h>
#include <string>
#include "TextureLoader.h"


class TextureClass final
{
public:
	static TextureClass* GetInstance()
	{
		if (!_pInstance)
		{
			_pInstance = new TextureClass();
		}
		return _pInstance;
	}

	TextureClass(const TextureClass&) = delete;
	TextureClass& operator=(const TextureClass&) = delete;
	~TextureClass();

	HRESULT SetTexture(std::string filePath,ID3D11Device* device,ID3D11DeviceContext* deviceContext);

private:
	TextureClass();
	static TextureClass* _pInstance;

	ID3D11ShaderResourceView* texture = nullptr;

	ID3D11ShaderResourceView* _TextureResource;
};

#endif