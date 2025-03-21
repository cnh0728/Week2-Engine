#pragma once

#include "Core/HAL/PlatformType.h"
#include <d3d11.h>
#include "Core/Math/Vector.h"
#include "Core/Rendering/LegacyTexture.h"
#include "Core/Container/String.h"

class UFont {
private:
	struct FontType {
		float left, right;
		int size;
	};

	struct VertexType {
		FVector position;
		FVector texture;
	};

public:
	UFont();
	~UFont();
	bool Create(ID3D11Device* Device, const wchar_t* FontFileName, const wchar_t* TextureFileName);
	void Release();
	void BuildVertexArray(void* Vertices, const FString Sentence, int ScreenWidth, int ScreenHeight);
	ID3D11ShaderResourceView* GetTexture() const { return Texture ? Texture->GetTexture() : nullptr; }

private:
	bool LoadFontData(const wchar_t* FontFileName);
	bool LoadTexture(ID3D11Device* Device, const wchar_t* TextureFileName);
	void ReleaseFontData();
	void ReleaseTexture();

private:
	FontType* Font;
	LegacyTexture* Texture;
};