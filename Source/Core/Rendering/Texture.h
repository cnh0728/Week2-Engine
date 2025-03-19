#pragma once
#include <Core/HAL/PlatformType.h>
#include <d3d11.h>
#include "Core/Math/Vector.h"

struct SubUV
{
	FVector2 U;
	FVector2 V;
};

class UTexture {
public:
	UTexture();
	~UTexture();
	static SubUV GetSubUV(int CharacterIndex, int AtlasWidth, int AtlasHeight, int CharWidth, int CharHeight);
	bool Create(ID3D11Device* Device, const wchar_t* TextureFileName);
	void Release();
	ID3D11ShaderResourceView* GetTexture() const { return Texture; }

private:
	ID3D11ShaderResourceView* Texture = nullptr;
};