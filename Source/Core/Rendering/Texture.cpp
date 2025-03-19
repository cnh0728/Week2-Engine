#include "Texture.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"

UTexture::UTexture()
{
	Texture = nullptr;
}

UTexture::~UTexture()
{
}

SubUV UTexture::GetSubUV(int CharacterIndex, int AtlasWidth, int AtlasHeight, int CharWidth, int CharHeight)
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

bool UTexture::Create(ID3D11Device* Device, const wchar_t* TextureFileName)
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

void UTexture::Release()
{
	if (Texture)
	{
		Texture->Release();
		Texture = nullptr;
	}
}
