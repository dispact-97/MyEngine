#include "TextureClass.h"

TextureClass::TextureClass()
	:_TextureResource()
{

}

TextureClass::TextureClass(const TextureClass& ohter)
	:_TextureResource()
{

}

TextureClass::~TextureClass()
{

}

HRESULT TextureClass::Initalize(Microsoft::WRL::ComPtr<ID3D11Device> device, WCHAR* file)
{
	HRESULT hr = S_OK;



	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void TextureClass::Shutdown()
{

}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return _TextureResource;
}
