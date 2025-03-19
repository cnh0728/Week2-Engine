#include "ParticleShader.h"
#include <d3dcompiler.h>

UParticleShader::UParticleShader()
{
	VertexShader = nullptr;
	PixelShader = nullptr;
	InputLayout = nullptr;
	ConstantBuffer = nullptr;
	SamplerState = nullptr;
}

UParticleShader::~UParticleShader()
{
}

bool UParticleShader::Create(ID3D11Device* Device, HWND hWindow)
{
	bool Result = CreateShader(Device, hWindow, L"Shaders/ShaderParticle.hlsl");
	if (!Result)
	{
		return false;
	}
	return true;
}

void UParticleShader::Release()
{
	ReleaseShader();
}

bool UParticleShader::Render(ID3D11DeviceContext* DeviceContext, uint32 IndexCount, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	bool Result = SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture);
	if (!Result)
	{
		return false;
	}
	RenderShader(DeviceContext, IndexCount);
	return true;
}

bool UParticleShader::CreateShader(ID3D11Device* Device, HWND hWindow, const WIDECHAR* ShaderFileName)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;
	ID3D10Blob* VertexShaderCSO = nullptr;
	ID3D10Blob* PixelShaderCSO = nullptr;
	uint32 NumElements;
	D3D11_BUFFER_DESC ConstantBufferDesc;
	D3D11_SAMPLER_DESC SamplerDesc;
	D3D11_BUFFER_DESC PixelBufferDesc;

	Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "ParticleVS", "vs_5_0", 0, 0, &VertexShaderCSO, &ErrorMessage);
	if (FAILED(Result))
	{
		OutputShaderErrorMessage(ErrorMessage, hWindow, ShaderFileName);
		return false;
	}

	Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "ParticlePS", "ps_5_0", 0, 0, &PixelShaderCSO, &ErrorMessage);
	if (FAILED(Result))
	{
		OutputShaderErrorMessage(ErrorMessage, hWindow, ShaderFileName);
		return false;
	}

	Result = Device->CreateVertexShader(VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), nullptr, &VertexShader);
	if (FAILED(Result))
	{
		return false;
	}

	Result = Device->CreatePixelShader(PixelShaderCSO->GetBufferPointer(), PixelShaderCSO->GetBufferSize(), nullptr, &PixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Result = Device->CreateInputLayout(Layout, ARRAYSIZE(Layout), VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), &InputLayout);
	if (FAILED(Result))
	{
		return false;
	}

	VertexShaderCSO->Release();
	VertexShaderCSO = nullptr;

	PixelShaderCSO->Release();
	PixelShaderCSO = nullptr;

	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDesc.ByteWidth = sizeof(Constants);
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&ConstantBufferDesc, nullptr, &ConstantBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Result = Device->CreateSamplerState(&SamplerDesc, &SamplerState);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void UParticleShader::ReleaseShader()
{
	if (SamplerState)
	{
		SamplerState->Release();
		SamplerState = nullptr;
	}
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}
	if (InputLayout)
	{
		InputLayout->Release();
		InputLayout = nullptr;
	}
	if (PixelShader)
	{
		PixelShader->Release();
		PixelShader = nullptr;
	}
	if (VertexShader)
	{
		VertexShader->Release();
		VertexShader = nullptr;
	}

	return;
}

bool UParticleShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Constants* DataPtr;
	uint32 BufferNumber;

	Result = DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	DataPtr = (Constants*)MappedResource.pData;

	DataPtr->MVP = FMatrix::Transpose(ProjectionMatrix) *
		FMatrix::Transpose(ViewMatrix) *
		FMatrix::Transpose(WorldMatrix);

	DeviceContext->Unmap(ConstantBuffer, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	DeviceContext->PSSetShaderResources(0, 1, &Texture);


	return true;
}

void UParticleShader::RenderShader(ID3D11DeviceContext* DeviceContext, uint32 IndexCount)
{
	DeviceContext->IASetInputLayout(InputLayout);
	DeviceContext->VSSetShader(VertexShader, nullptr, 0);
	DeviceContext->PSSetShader(PixelShader, nullptr, 0);
	DeviceContext->PSSetSamplers(0, 1, &SamplerState);
	DeviceContext->DrawIndexed(IndexCount, 0, 0);

	return;
}

void UParticleShader::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, const wchar_t* ShaderFileName)
{
	char* CompileErrors;
	uint32 BufferSize, i;
	std::ofstream fout;

	CompileErrors = (char*)(ErrorMessage->GetBufferPointer());
	BufferSize = ErrorMessage->GetBufferSize();

	fout.open("shader-error.txt");
	for (i = 0; i < BufferSize; i++)
	{
		fout << CompileErrors[i];
	}

	fout.close();

	ErrorMessage->Release();
	ErrorMessage = nullptr;

	MessageBox(Hwnd, L"Error compiling shader. Check ShaderError.txt for message.", ShaderFileName, MB_OK);

	return;
}
