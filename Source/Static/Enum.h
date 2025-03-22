#pragma once

enum class EViewModeIndex : uint32
{
    VMI_Lit,
    VMI_Unlit,
    VMI_Wireframe,
};

enum ETextureResource : uint32
{
    ECat = 0,
    EEarth = 1,
    ECustom = 10,
};

enum EPixelType : uint32
{
    EDefalutColor,
    ECustomColor,
    ETexture,
};



