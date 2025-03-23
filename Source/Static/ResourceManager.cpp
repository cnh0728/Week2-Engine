// ResourceManager.cpp
#include "ResourceManager.h"
#include "directxtk/WICTextureLoader.h"
#include "Data/MtlLoader.h"

void UResourceManager::Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InContext)
{
    Device = InDevice;
    DeviceContext = InContext;
}

void UResourceManager::Shutdown()
{
    ReleaseAllTextures();
    Device = nullptr;
    DeviceContext = nullptr;
}
void UResourceManager::LoadTexture(ETextureResource Type, std::string Path)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &Path[0], (int)Path.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &Path[0], (int)Path.size(), &wstr[0], size_needed);
    const wchar_t* TexturePathWCHAR = wstr.c_str();

    // 텍스처 로드
    ID3D11ShaderResourceView* ResourceView = nullptr;
    HRESULT hr = DirectX::CreateWICTextureFromFile(Device, DeviceContext, TexturePathWCHAR, nullptr, &ResourceView);
    if (SUCCEEDED(hr))
    {
        TextureResources[Type] = ResourceView;
    }
}



ID3D11ShaderResourceView* UResourceManager::GetTexture(ETextureResource Type) const
{
    if (TextureResources.Contains(Type))
    {
        return TextureResources[Type];
    }
    return nullptr;
}

void UResourceManager::ReleaseAllTextures()
{
    for (auto& [Type, Texture] : TextureResources)
    {
        if (Texture)
        {
            Texture->Release();
        }
    }
    TextureResources.Empty();
}

const FMaterialData* UResourceManager::GetMaterial(const std::string& name) const {
    return Materials.Find(name);
