#pragma once

#include <string>

#include "Core/HAL/PlatformType.h"
#include "Core/Math/Vector.h"

struct UObjectInfo
{
    FVector Location; 
    FVector Rotation;
    FVector Scale;
    std::string ObjectType;

    uint32 UUID;
};

struct UWorldInfo
{
    UObjectInfo** ObjctInfos;
    uint32 ActorCount;
    uint32 Version;
    std::string SceneName;
};

class JsonSaveHelper
{
public:
    // SceneName - 확장자 제외
    static UWorldInfo* LoadScene(std::string SceneName);
    static void SaveScene(const UWorldInfo& WorldInfo);
};
