#pragma	once
#include "Core/HAL/PlatformType.h"
#include <d3d11.h>
#include <fstream>
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector.h"


class UFontShader
{
private:
	struct Constants {
		FMatrix MVP;
	};

	struct PixelBufferType {
		FVector4 Color;
	};

public:
	UFontShader();
	~UFontShader();

	/** Renderer를 초기화 합니다. */
	bool Create(ID3D11Device* Device, HWND hWindow);

	/** Renderer에 사용된 모든 리소스를 해제합니다. */
	void Release();

	bool Render(ID3D11DeviceContext* DeviceContext, uint32 IndexCount, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture, FVector4 Color);
	static void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, const wchar_t* ShaderFileName);

private:
	bool CreateShader(ID3D11Device* Device, HWND hWindow, const wchar_t* ShaderFileName);

	void ReleaseShader();
	bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture, FVector4 Color);
	void RenderShader(ID3D11DeviceContext* DeviceContext, uint32 IndexCount);

private:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* InputLayout = nullptr;
	ID3D11Buffer* ConstantBuffer = nullptr;
	ID3D11SamplerState* SamplerState = nullptr;
	ID3D11Buffer* PixelBuffer = nullptr;
};