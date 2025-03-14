#include "Texture.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"

#if defined(DEBUG) || defined(_DEBUG)
#define ENABLE_DEBUG_LAYER
#endif

UTexture::UTexture()
{
	Texture = nullptr;
}

UTexture::~UTexture()
{
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
	//Result = DirectX::LoadFromTGAFile(TextureFileName, nullptr, Image);
	Result = DirectX::LoadFromWICFile(L"Source/Core/Rendering/Text/font01.dds", DirectX::WIC_FLAGS_NONE, nullptr, Image);
	//Result = DirectX::LoadFromDDSFile(TextureFileName, DirectX::DDS_FLAGS_NONE, nullptr, Image);
	if (FAILED(Result))
	{
		return false;
	}

	// 텍스처 생성
	ID3D11Resource* TextureResource;
	Result = DirectX::CreateTexture(Device, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &TextureResource);
	PrintErrorMessage(Result);
	if (FAILED(Result))
	{
		return false;
	}
#ifdef ENABLE_DEBUG_LAYER
	// 셰이더 리소스 뷰 생성
	Result = Device->CreateShaderResourceView(TextureResource, nullptr, &Texture);
	PrintErrorMessage(Result);
#endif
	// 텍스쳐 리스소 해제
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
