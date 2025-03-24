// CustomComponent.h
#pragma once
#include "TextureComponent.h"
#include "UPrimitiveComponent.h"
#include "Data/MaterialData.h"
#include "Primitive/PrimitiveVertices.h"
#include "Static/ResourceManager.h"
#include "Data/ObjLoader.h"
#include "Object/ObjectMacros.h"

struct FRenderUnit {
    TArray<FVertexSimple> Vertices;
    TArray<uint32_t> Indices;
    const FMaterialData* Material;
};

class UCustomComponent : public UPrimitiveComponent
{
public:
    using Super = UPrimitiveComponent;
    DECLARE_CLASS(UCustomComponent, UPrimitiveComponent)

    UCustomComponent() {
        bIsDefaultRendered = true;
        bCanPick = true;
        SetPixelType(EPixelType::ETexture);
    }

    virtual ~UCustomComponent() = default;

    EPrimitiveType GetType() override {
        return EPrimitiveType::EPT_Custom;
    }

    void LoadFromObj(const std::string& path);
  
    const TArray<FRenderUnit>& GetRenderUnits() const { return RenderUnits; }

private:
    TArray<FRenderUnit> RenderUnits;
};