#pragma once
#include "TextureComponent.h"
#include "UPrimitiveComponent.h"

class UCustomComponent : public UPrimitiveComponent
{
public:
    using Super = UPrimitiveComponent;
    DECLARE_CLASS(UCustomComponent, UPrimitiveComponent)
public:
    UCustomComponent()
    {
        bIsDefaultRendered = true;
        bCanPick = true;
    }
    virtual ~UCustomComponent() = default;
    EPrimitiveType GetType() override
    {
        return EPrimitiveType::EPT_Custom;
    }
};