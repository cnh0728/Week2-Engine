#pragma once
#include "Core/HAL/PlatformType.h"
#include <d3d11.h>
#include <fstream>
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector.h"

class UTextureRenderer {
private:
	struct Constants {
		FMatrix MVP;
	};

public:
	UTextureRenderer();
	~UTextureRenderer();

	bool Create(ID3D11Device* Device, HWND hWindow);
	void Release();

	bool Render(ID3D11DeviceContext* DeviceContext, uint32 IndexCount, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture);
	static void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, const wchar_t* ShaderFileName);

private:
	bool CreateShader(ID3D11Device* Device, HWND HWindow, const wchar_t* ShaderFileName);
	void ReleaseShader();
	bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture);
	void RenderShader(ID3D11DeviceContext* DeviceContext, uint32 IndexCount);

private:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* InputLayout = nullptr;
	ID3D11Buffer* ConstantBuffer = nullptr;
	ID3D11SamplerState* SamplerState = nullptr;
};
