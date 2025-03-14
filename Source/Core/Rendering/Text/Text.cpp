#include "Text.h"

UText::UText()
{
	Font = nullptr;
	FontShader = nullptr;
	ScreenWidth = 0;
	ScreenHeight = 0;
	BaseViewMatrix = FMatrix();
	Sentence1 = nullptr;
	Sentence2 = nullptr;
}

UText::~UText()
{
}

bool UText::Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND Hwnd, uint32 ScreenWidth, uint32 ScreenHeight, FMatrix BaseViewMatrix)
{
	bool Result;
	this->ScreenWidth = ScreenWidth;
	this->ScreenHeight = ScreenHeight;
	this->BaseViewMatrix = BaseViewMatrix;
	Font = new UFont();
	if (!Font)
	{
		return false;
	}
	Result = Font->Create(Device, L"Source/Core/Rendering/Text/font01.txt", L"Source/Core/Rendering/Text/font01.tga");
	if (!Result)
	{
		return false;
	}

	FontShader = new UFontShader();
	if (!FontShader)
	{
		return false;
	}

	Result = FontShader->Create(Device, Hwnd);
	if (!Result)
	{
		return false;
	}

	Result = InitializeSentence(&Sentence1, 16, Device);
	if (!Result)
	{
		return false;
	}

	Result = UpdateSentence(Sentence1, "Hello World", 100, 100, 1.0f, 1.0f, 1.0f, DeviceContext);
	if (!Result)
	{
		return false;
	}
	
	Result = InitializeSentence(&Sentence2, 16, Device);
	if (!Result)
	{
		return false;
	}

	Result = UpdateSentence(Sentence2, "Goodbye World", 100, 200, 1.0f, 1.0f, 0.0f, DeviceContext);
	if (!Result)
	{
		return false;
	}

	return true;
}

void UText::Release()
{
	ReleaseSentence(&Sentence1);
	ReleaseSentence(&Sentence2);
	if (FontShader)
	{
		FontShader->Release();
		delete FontShader;
		FontShader = nullptr;
	}
	if (Font)
	{
		Font->Release();
		delete Font;
		Font = nullptr;
	}
}

bool UText::Render(ID3D11DeviceContext* DeviceContext, FMatrix WorldMatrix, FMatrix OrthoMatrix)
{
	bool Result;
	Result = RenderSentence(DeviceContext, Sentence1, WorldMatrix, OrthoMatrix);
	if (!Result)
	{
		return false;
	}
	Result = RenderSentence(DeviceContext, Sentence2, WorldMatrix, OrthoMatrix);
	if (!Result)
	{
		return false;
	}
	return true;
}

bool UText::InitializeSentence(SentenceType** Sentence, int MaxLength, ID3D11Device* Device)
{
	VertexType* Vertices;
	uint32* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT Result;
	*Sentence = new SentenceType();
	if (!*Sentence)
	{
		return false;
	}
	(*Sentence)->VertexCount = 6 * MaxLength;
	(*Sentence)->IndexCount = (*Sentence)->VertexCount;
	(*Sentence)->MaxLength = MaxLength;
	(*Sentence)->Red = 1.0f;
	(*Sentence)->Green = 1.0f;
	(*Sentence)->Blue = 1.0f;
	Vertices = new VertexType[(*Sentence)->VertexCount];
	if (!Vertices)
	{
		return false;
	}
	Indices = new uint32[(*Sentence)->IndexCount];
	if (!Indices)
	{
		return false;
	}
	memset(Vertices, 0, (sizeof(VertexType) * (*Sentence)->VertexCount));
	for (int i = 0; i < (*Sentence)->IndexCount; i++)
	{
		Indices[i] = i;
	}
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * (*Sentence)->VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &(*Sentence)->VertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(uint32) * (*Sentence)->IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &(*Sentence)->IndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	delete[] Vertices;
	Vertices = nullptr;
	delete[] Indices;
	Indices = nullptr;

	return true;
}

bool UText::UpdateSentence(SentenceType* Sentence, const char* Text, float DrawX, float DrawY, float Red, float Green, float Blue, ID3D11DeviceContext* DeviceContext)
{
	int NumLetters;
	VertexType* Vertices;
	float X, Y;
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VertexPtr;

	Sentence->Red = Red;
	Sentence->Green = Green;
	Sentence->Blue = Blue;

	NumLetters = (int)strlen(Text);

	if (NumLetters > Sentence->MaxLength)
	{
		return false;
	}

	Vertices = new VertexType[Sentence->VertexCount];
	if (!Vertices)
	{
		return false;
	}

	memset(Vertices, 0, (sizeof(VertexType) * Sentence->VertexCount));
	X = (float)(((ScreenWidth / 2) * -1) + DrawX);
	Y = (float)((ScreenHeight / 2) - DrawY);

	Font->BuildVertexArray((void*)Vertices, Text, X, Y);

	Result = DeviceContext->Map(Sentence->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	VertexPtr = (VertexType*)MappedResource.pData;

	memcpy(VertexPtr, (void*)Vertices, (sizeof(VertexType) * Sentence->VertexCount));

	DeviceContext->Unmap(Sentence->VertexBuffer, 0);

	delete[] Vertices;
	Vertices = nullptr;

	return true;
}

void UText::ReleaseSentence(SentenceType** Sentence)
{
	if (*Sentence)
	{
		if ((*Sentence)->VertexBuffer)
		{
			(*Sentence)->VertexBuffer->Release();
			(*Sentence)->VertexBuffer = nullptr;
		}
		if ((*Sentence)->IndexBuffer)
		{
			(*Sentence)->IndexBuffer->Release();
			(*Sentence)->IndexBuffer = nullptr;
		}
		delete* Sentence;
		*Sentence = nullptr;
	}
}

bool UText::RenderSentence(ID3D11DeviceContext* DeviceContext, SentenceType* Sentence, FMatrix WorldMatrix, FMatrix OrthoMatrix)
{
	uint32 Stride;
	uint32 Offset;
	FVector4 PixelColor;
	bool Result;

	Stride = sizeof(VertexType);
	Offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &Sentence->VertexBuffer, &Stride, &Offset);
	DeviceContext->IASetIndexBuffer(Sentence->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	PixelColor = FVector4(Sentence->Red, Sentence->Green, Sentence->Blue, 1.0f);
	Result = FontShader->Render(DeviceContext, Sentence->IndexCount, WorldMatrix, BaseViewMatrix, OrthoMatrix, Font->GetTexture(), PixelColor);

	if (!Result)
	{
		return false;
	}

	return true;
}