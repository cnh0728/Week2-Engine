#pragma once
#include "Core/HAL/PlatformType.h"
#include <d3d11.h>
#include "Core/Math/Vector.h"
#include "Font.h"
#include "FontShader.h"
#include "Core/Math/Matrix.h"


class UText {
private:
	struct SentenceType {
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		int VertexCount;
		int IndexCount;
		int MaxLength;
		float Red, Green, Blue;
	};

	struct VertexType {
		FVector Position;
		FVector Texture;
	};

public:
	UText();
	~UText();
	bool Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND Hwnd, uint32 ScreenWdith, uint32 ScreenHeight, FMatrix BaseViewMatrix);
	void Release();
	bool Render(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix OrthoMatrix);
	UFont* GetFont() const { return Font; }

private:
	bool InitializeSentence(SentenceType** Sentence, int MaxLength, ID3D11Device* Device);
	bool UpdateSentence(SentenceType* Sentence, const char* Text, float DrawX, float DrawY, float Red, float Green, float Blue, ID3D11DeviceContext* DeviceContext);
	void ReleaseSentence(SentenceType** Sentence);
	bool RenderSentence(ID3D11DeviceContext* DeviceContext, SentenceType* Sentence, FMatrix WorldMatrix, FMatrix OrthoMatrix);

private:
	UFont* Font;
	UFontShader* FontShader;
	uint32 ScreenWidth, ScreenHeight;
	FMatrix BaseViewMatrix;
	SentenceType* Sentence1;
	SentenceType* Sentence2;
};