#include "FontShader.h"
#include <d3dcompiler.h>

UFontShader::UFontShader()
{
	VertexShader = nullptr;
	PixelShader = nullptr;
	InputLayout = nullptr;
	ConstantBuffer = nullptr;
}

UFontShader::~UFontShader()
{
}

bool UFontShader::Create(ID3D11Device* Device, HWND HWindow)
{
	bool Result = CreateShader(Device, HWindow, L"Shaders/ShaderFont.hlsl");
	if (!Result) {
		return false;
	}

	return true;
}

void UFontShader::Release()
{
	ReleaseShader();
}

bool UFontShader::Render(ID3D11DeviceContext* DeviceContext, uint32 IndexCount, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture, FVector4 Color)
{
	bool result;

	result = SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture, Color);
	if (!result)
	{
		return false;
	}

	RenderShader(DeviceContext, IndexCount);

	return true;
}



bool UFontShader::CreateShader(ID3D11Device* Device, HWND HWindow, const wchar_t* ShaderFileName)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;
	ID3D10Blob* VertexShaderCSO = nullptr;
	ID3D10Blob* PixelShaderCSO = nullptr;
	uint32 NumElements;
	D3D11_BUFFER_DESC DonstantBufferDesc;
	D3D11_SAMPLER_DESC SamplerDesc;
	D3D11_BUFFER_DESC PixelBufferDesc;

	Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "FontVS", "vs_5_0", 0, 0, &VertexShaderCSO, &ErrorMessage);
	Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "FontPS", "ps_5_0", 0, 0, &PixelShaderCSO, &ErrorMessage);

	if (FAILED(Result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, HWindow, ShaderFileName);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(HWindow, ShaderFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	Result = Device->CreateVertexShader(VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), nullptr, &VertexShader);
	Result = Device->CreatePixelShader(PixelShaderCSO->GetBufferPointer(), PixelShaderCSO->GetBufferSize(), nullptr, &PixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	// 입력 레이아웃 정의 및 생성
	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Device->CreateInputLayout(Layout, ARRAYSIZE(Layout), VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), &InputLayout);
	if (FAILED(Result))
	{
		return false;
	}

	VertexShaderCSO->Release();
	PixelShaderCSO->Release();

	// 상수 버퍼 생성
	DonstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	DonstantBufferDesc.ByteWidth = sizeof(Constants);
	DonstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	DonstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DonstantBufferDesc.MiscFlags = 0;
	DonstantBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&DonstantBufferDesc, nullptr, &ConstantBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 샘플러 상태 생성
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

	PixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	PixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PixelBufferDesc.MiscFlags = 0;
	PixelBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&PixelBufferDesc, nullptr, &PixelBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void UFontShader::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, const wchar_t* ShaderFileName)
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
	MessageBox(Hwnd, L"Error compiling shader. Check shader-error.txt for message.", ShaderFileName, MB_OK);
}

void UFontShader::ReleaseShader()
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
}

bool UFontShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix, ID3D11ShaderResourceView* Texture, FVector4 Color)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Constants* DataPtr;
	PixelBufferType* DataPtr2;
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
	BufferNumber = 0;
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &ConstantBuffer);
	DeviceContext->PSSetShaderResources(0, 1, &Texture);

	Result = DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	DataPtr2 = (PixelBufferType*)MappedResource.pData;
	DataPtr2->Color = Color;
	DeviceContext->Unmap(ConstantBuffer, 0);
	BufferNumber = 0;
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &ConstantBuffer);
	
	return true;
}



void UFontShader::RenderShader(ID3D11DeviceContext* DeviceContext, uint32 IndexCount)
{
	DeviceContext->IASetInputLayout(InputLayout);

	DeviceContext->VSSetShader(VertexShader, nullptr, 0);
	DeviceContext->PSSetShader(PixelShader, nullptr, 0);

	DeviceContext->PSSetSamplers(0, 1, &SamplerState);

	DeviceContext->DrawIndexed(IndexCount, 0, 0);

	return;
}

