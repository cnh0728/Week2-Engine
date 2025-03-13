#pragma once
#include <Core/HAL/PlatformType.h>
#include <d3d11.h>


class UTexture {
public:
	UTexture();
	~UTexture();
	bool Create(ID3D11Device* Device, const wchar_t* TextureFileName);
	void Release();
	ID3D11ShaderResourceView* GetTexture() const { return Texture; }

private:
	ID3D11ShaderResourceView* Texture;
};