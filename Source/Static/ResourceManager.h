#pragma once
#define _TCHAR_DEFINED 
#include <d3d11.h>
#include "Core/HAL/PlatformMemory.h"
#include "Static/Enum.h"
#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"
#include "Data/MaterialData.h"
#include "Primitive/PrimitiveVertices.h"
#include "Data/MtlLoader.h"



class UResourceManager : public TSingleton<UResourceManager>
{
public:
    UResourceManager() = default;
    ~UResourceManager() = default;

    void Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InContext);
    void Shutdown();

    void LoadTexture(const std::string& Path);
    ID3D11ShaderResourceView* GetTexture(const std::string& Path) const;
    void ReleaseAllTextures();

    const FMaterialData* GetMaterial(const std::string& name) const;
    void SetMaterial(const std::string& name, const FMaterialData& materialData);



private:
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
   

public:
    void SetMeshData(const std::string& path, const TArray<FSubMeshData>& meshData);
    const TArray<FSubMeshData>* GetMeshData(const std::string& path) const;
    bool HasMeshData(const std::string& path) const;


private:
    TMap<std::string, ID3D11ShaderResourceView*> TextureMap;
    TMap<std::string, FMaterialData> Materials;
    TMap<std::string, TArray<FSubMeshData>> Meshes;  //(obj 파일이름 / 오브젝트의 서브메쉬 데이터들)을 맵에 저장

public:
    bool LoadMtlFile(const std::string& path); // public wrapper

private:
    MtlLoader MaterialLoader;

};


