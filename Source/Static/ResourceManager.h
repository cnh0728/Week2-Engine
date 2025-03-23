#pragma once
#define _TCHAR_DEFINED 
#include <d3d11.h>
#include "Core/HAL/PlatformMemory.h"
#include "Static/Enum.h"
#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"

struct FMaterialData;

class UResourceManager : public TSingleton<UResourceManager>
{
public:
    void Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InContext);
    void Shutdown();

    void LoadTexture(ETextureResource Type, std::string Path);
    ID3D11ShaderResourceView* GetTexture(ETextureResource Type) const;
    const FMaterialData* GetMaterial(const std::string& name) const;
    const FMaterialData* SetMaterial(const std::string& name, FMaterialData) const;

    void ReleaseAllTextures();
    UResourceManager() = default;
    ~UResourceManager() = default;

private:
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    TMap<ETextureResource, ID3D11ShaderResourceView*> TextureResources;
private:
    TMap<std::string, FMaterialData> Materials;
};


