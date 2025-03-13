
#include "Texture.h"
//#include "ThirdParty/DirectXTex/Include/DirectXTex.h"


UTexture::UTexture()
{
	Texture = nullptr;
}

UTexture::~UTexture()
{
}

bool UTexture::Create(ID3D11Device* Device, const wchar_t* TextureFileName)
{
	//HRESULT Result;
	//DirectX::ScratchImage Image;

	//// 이미지 파일 로드
	//Result = DirectX::LoadFromWICFile(TextureFileName, DirectX::WIC_FLAGS_NONE, nullptr, Image);
	//if (FAILED(Result))
	//{
	//	return false;
	//}
	//return true;

	//// 텍스처 생성
	//ID3D11Resource* TextureResource;
	//Result = DirectX::CreateTexture(Device, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &TextureResource);
	//if (FAILED(Result))
	//{
	//	return false;
	//}

	//// 셰이더 리소스 뷰 생성
	//Result = Device->CreateShaderResourceView(TextureResource, nullptr, &Texture);

	//// 텍스쳐 리스소 해제
	//TextureResource->Release();

	//if (FAILED(Result))
	//{
	//	return false;
	//}

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
