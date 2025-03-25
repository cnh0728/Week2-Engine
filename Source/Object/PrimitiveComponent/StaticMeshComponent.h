// CustomComponent.h
#pragma once
#include "TextureComponent.h"
#include "UPrimitiveComponent.h"
#include "Data/MaterialData.h"
#include "Primitive/PrimitiveVertices.h"
#include "Static/ResourceManager.h"
#include "Data/ObjManager.h"
#include "Object/ObjectMacros.h"

struct FStaticMesh {
    TArray<FVertexPNCT> Vertices;
    TArray<uint32_t> Indices;
    const FMaterialData* Material;
};

class UStaticMeshComponent : public UPrimitiveComponent
{
public:
    using Super = UPrimitiveComponent;
    DECLARE_CLASS(UStaticMeshComponent, UPrimitiveComponent)

    UStaticMeshComponent() {
        bIsDefaultRendered = true;
        bCanPick = true;
        SetPixelType(EPixelType::ETexture);
    }

    virtual ~UStaticMeshComponent() = default;

    EPrimitiveType GetType() override {
        return EPrimitiveType::EPT_Custom;
    }

    void LoadFromObj(const std::string& path);
  
    const TArray<FStaticMesh>& GetRenderUnits() const { return RenderUnits; }

private:
    TArray<FStaticMesh> RenderUnits;
};