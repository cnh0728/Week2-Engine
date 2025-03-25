#pragma once
#include <Core/HAL/PlatformType.h>
#include <d3d11.h>
#include <fstream>
#include "Core/Math/Vector.h"
#include "Primitive/PrimitiveVertices.h"

struct SubUV
{
	FVector2 U;
	FVector2 V;
};

class LegacyTexture {
public:
	LegacyTexture();
	~LegacyTexture();
	static SubUV GetSubUV(int CharacterIndex, int AtlasWidth, int AtlasHeight, int CharWidth, int CharHeight);
	bool Create(ID3D11Device* Device, const wchar_t* TextureFileName);
	void Release();
	void Render(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
	ID3D11ShaderResourceView* GetTexture() const { return Texture; }

private:
	ID3D11ShaderResourceView* Texture = nullptr;
	TArray<FVertexPNCT> Vertices;
	TArray<uint32> Indices;
	ID3D11Buffer* VertexBuffer, * IndexBuffer;
};