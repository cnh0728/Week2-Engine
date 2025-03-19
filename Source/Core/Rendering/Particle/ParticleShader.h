#pragma once
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector.h"
#include <d3d11.h>
#include <fstream>


class UParticleShader {
private:
	struct Constants {
		FMatrix MVP;
	};

public:
	UParticleShader();
	~UParticleShader();

	bool Create(ID3D11Device* Device, HWND hWindow);
	void Release();
	bool Render(ID3D11DeviceContext* DeviceContext, uint32 IndexCount, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture);

private:
	bool CreateShader(ID3D11Device* Device, HWND hWindow, const wchar_t* ShaderFileName);
	void ReleaseShader();
	bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture);
	void RenderShader(ID3D11DeviceContext* DeviceContext, uint32 IndexCount);
	void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, const wchar_t* ShaderFileName);

private:
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;
	ID3D11Buffer* ConstantBuffer;
	ID3D11SamplerState* SamplerState;
};