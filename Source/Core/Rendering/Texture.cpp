#include "Texture.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"
#include <wincodec.h>
#include <wrl.h>

using namespace Microsoft::WRL;


UTexture::UTexture()
{
		
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

	//// WIC 팩토리 생성
	//ComPtr<IWICImagingFactory> wicFactory;
	//HRESULT hr = CoCreateInstance(
	//	CLSID_WICImagingFactory,
	//	nullptr,
	//	CLSCTX_INPROC_SERVER,
	//	IID_PPV_ARGS(&wicFactory));

	//if (FAILED(hr)) return hr;

	//// 이미지 디코더 생성
	//ComPtr<IWICBitmapDecoder> decoder;
	//hr = wicFactory->CreateDecoderFromFilename(
	//	TextureFileName,
	//	nullptr,
	//	GENERIC_READ,
	//	WICDecodeMetadataCacheOnDemand,
	//	&decoder);

	//if (FAILED(hr)) return hr;

	//// 첫 번째 프레임(이미지) 가져오기
	//ComPtr<IWICBitmapFrameDecode> frame;
	//hr = decoder->GetFrame(0, &frame);
	//if (FAILED(hr)) return hr;

	//// 포맷 변환
	//ComPtr<IWICFormatConverter> formatConverter;
	//hr = wicFactory->CreateFormatConverter(&formatConverter);
	//if (FAILED(hr)) return hr;

	//hr = formatConverter->Initialize(
	//	frame.Get(),
	//	GUID_WICPixelFormat32bppRGBA,  // DirectX에서 지원되는 RGBA 포맷
	//	WICBitmapDitherTypeNone,
	//	nullptr,
	//	0.0,
	//	WICBitmapPaletteTypeCustom);

	//if (FAILED(hr)) return hr;

	//// 이미지 크기 가져오기
	//UINT width, height;
	//hr = frame->GetSize(&width, &height);
	//if (FAILED(hr)) return hr;

	//// WIC에서 픽셀 데이터를 가져오기 위해 메모리 할당
	//std::vector<BYTE> imageData(width * height * 4); // RGBA 32비트

	//hr = formatConverter->CopyPixels(
	//	nullptr,
	//	width * 4,
	//	static_cast<UINT>(imageData.size()),
	//	imageData.data());

	//if (FAILED(hr)) return hr;

	//// DirectX 텍스처 생성
	//D3D11_TEXTURE2D_DESC textureDesc = {};
	//textureDesc.Width = width;
	//textureDesc.Height = height;
	//textureDesc.MipLevels = 1;
	//textureDesc.ArraySize = 1;
	//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//textureDesc.SampleDesc.Count = 1;
	//textureDesc.Usage = D3D11_USAGE_DEFAULT;
	//textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//D3D11_SUBRESOURCE_DATA initData = {};
	//initData.pSysMem = imageData.data();
	//initData.SysMemPitch = width * 4;

	//ID3D11Texture2D* texture;
	//hr = Device->CreateTexture2D(&textureDesc, &initData, &texture);
	//if (FAILED(hr)) return hr;

	//// Shader Resource View 생성
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = textureDesc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = 1;

	//ID3D11ShaderResourceView* Texture2 = nullptr;
	//hr = Device->CreateShaderResourceView(texture, nullptr, &Texture);
	//PrintErrorMessage(hr);
	//return true;

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
	//ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(TextureResource);
	//D3D11_TEXTURE2D_DESC desc;
	//texture2D->GetDesc(&desc);
	//PrintErrorMessage(Result);

	//char buffer[256];
	//sprintf_s(buffer, "Texture2D - Width: %u, Height: %u, MipLevels: %u, Format: %u\n",
	//	desc.Width, desc.Height, desc.MipLevels, desc.Format);
	//OutputDebugStringA(buffer);

	//if (FAILED(Result))
	//{
	//	return false;
	//}

	//// 셰이더 리소스 뷰 생성
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//ZeroMemory(&srvDesc, sizeof(srvDesc));

	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // 뷰의 종류
	//srvDesc.Texture2D.MostDetailedMip = 0; // 사용할 가장 자세한 Mipmap 수준의 인덱스
	//srvDesc.Texture2D.MipLevels = 1; // 텍스처 뷰의 최대 Mipmap 수준

	//Result = Device->CreateShaderResourceView(TextureResource, &srvDesc, &Texture);
	//PrintErrorMessage(Result);

	//// 텍스처 리소스 해제
	//TextureResource->Release();

	//if (FAILED(Result))
	//{
	//	return false;
	//}

}

void UTexture::Release()
{
	if (Texture)
	{
		Texture->Release();
		Texture = nullptr;
	}
}
