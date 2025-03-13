#pragma once

#include <d3d11.h>
#include "Core/Math/Vector.h"


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
	bool Create(ID3D11Device* Device, const char* FontFileName, const char* TextureFileName);
	void Release();
	void BuildVertexArray(void* Vertices, const char* Sentence, float DrawX, float DrawY);
	ID3D11ShaderResourceView* GetTexture() const { return Texture; }

private:
	bool LoadFontData(const char* FontFileName);
	bool LoadTexture(ID3D11Device* Device, const char* TextureFileName);
	void ReleaseFontData();
	void ReleaseTexture();

private:
	FontType* Font;
	ID3D11ShaderResourceView* Texture;
};