#pragma once
#include "UPrimitiveComponent.h"

class UTextureComponent : public UPrimitiveComponent
{
    using Super = UPrimitiveComponent;
    DECLARE_CLASS(UTextureComponent, UPrimitiveComponent)

public:
    UTextureComponent()
    {
        bIsDefaultRendered = true;
        bCanPick = true;
    }
    virtual ~UTextureComponent() = default;
    EPrimitiveType GetType()override
    {
        return EPrimitiveType::EPT_Texture;
    }
};
