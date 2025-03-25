#include "LegacyTexture.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"

LegacyTexture::LegacyTexture()
{
	Texture = nullptr;
}

LegacyTexture::~LegacyTexture()
{
}

SubUV LegacyTexture::GetSubUV(int CharacterIndex, int AtlasWidth, int AtlasHeight, int CharWidth, int CharHeight)
{
	int Columns = AtlasWidth / CharWidth;
	int Rows = AtlasHeight / CharHeight;

	int Column = CharacterIndex % Columns;
	int Row = CharacterIndex / Columns;

	SubUV UV;
	UV.U = FVector2(Column * CharWidth / (float)AtlasWidth, (Column + 1) * CharWidth / (float)AtlasWidth);
	UV.V = FVector2(Row * CharHeight / (float)AtlasHeight, (Row + 1) * CharHeight / (float)AtlasHeight);

	return UV;
}

void PrintErrorMessage(HRESULT hr)
{
	char* errorMsg = nullptr;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorMsg,
		0,
		nullptr
	);

	if (errorMsg)
	{
		OutputDebugStringA(errorMsg);
		LocalFree(errorMsg);
	}
}

bool LegacyTexture::Create(ID3D11Device* Device, const wchar_t* TextureFileName)
{
	HRESULT Result;
 	DirectX::ScratchImage Image;

	// 이미지 파일 로드
	Result = DirectX::LoadFromTGAFile(TextureFileName, nullptr, Image);
	if (FAILED(Result))
	{
		PrintErrorMessage(Result);
		return false;
	}

	// Image 객체의 유효성 확인
	if (Image.GetImageCount() == 0 || Image.GetImages() == nullptr)
	{
		OutputDebugStringA("Invalid image data.\n");
		return false;
	}

	// 텍스처 생성
	ID3D11Resource* TextureResource;
	Result = DirectX::CreateTexture(Device, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &TextureResource);
	if (FAILED(Result))
	{
		PrintErrorMessage(Result);
		return false;
	}

	// 셰이더 리소스 뷰 생성
	Result = Device->CreateShaderResourceView(TextureResource, nullptr, &Texture);
	PrintErrorMessage(Result);

	// 텍스처 리소스 해제
	TextureResource->Release();

	if (FAILED(Result))
	{
		return false;
	}

	return true;

}

void LegacyTexture::Release()
{
	if (Texture)
	{
		Texture->Release();
		Texture = nullptr;
	}
}

void LegacyTexture::Render(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	FVertexPNCT* VerticesPtr;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;

	Vertices = OriginVertices[EPrimitiveType::EPT_Texture];
	Indices = OriginIndices[EPrimitiveType::EPT_Texture];

	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(FVertexPNCT) * Vertices.Num();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexData.pSysMem = Vertices.GetData();
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	Device->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer);

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(uint32) * Indices.Num();
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexData.pSysMem = Indices.GetData();
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer);

	uint32 Stride = sizeof(FVertexPNCT);;
	uint32 Offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	VerticesPtr = (FVertexPNCT*)MappedResource.pData;

	memcpy(VerticesPtr, Vertices.GetData(), Vertices.Num() * (sizeof(FVertexPNCT)));

	DeviceContext->Unmap(VertexBuffer, 0);
}