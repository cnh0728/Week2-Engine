#include "JsonSaveHelper.h"

#include <fstream>

#include "Core/EngineStatics.h"
#include "Debug/DebugConsole.h"
#include "SimpleJSON/Json.hpp"

using json::JSON;

// SceneName - 확장자 제외
UWorldInfo* JsonSaveHelper::LoadScene(std::string SceneName)
{
    std::ifstream Input(SceneName + ".scene");

    if (!Input.is_open())
    {
        UE_LOG("Scene file not found");
        return nullptr;
    }
    std::string Contents;
    Input.seekg( 0, std::ios::end );
    Contents.reserve( Input.tellg() );
    Input.seekg( 0, std::ios::beg );

    Contents.assign( std::istreambuf_iterator<char>( Input ), std::istreambuf_iterator<char>() );

    JSON Json = JSON::Load(Contents);

    UWorldInfo* WorldInfo = new UWorldInfo();
     
    WorldInfo->Version = Json["Version"].ToInt();
    WorldInfo->SceneName = Json["SceneName"].ToString();
    WorldInfo->ActorCount = Json["ActorCount"].ToInt();
    WorldInfo->ObjctInfos = new UObjectInfo*[WorldInfo->ActorCount];
    uint32 NextUUID = Json["NextUUID"].ToInt();
    NextUUID = FMath::Max(NextUUID, WorldInfo->ActorCount);
    int index = 0;
    for (int i = 0; i < NextUUID ; i++){
        if (!Json["Actors"].hasKey(std::to_string(i)))
        {
            continue;
        }
        std::string UUID = std::to_string(i);
        
        UObjectInfo* ObjectInfo = new UObjectInfo();
        WorldInfo->ObjctInfos[index] = ObjectInfo;
        
        JSON Location = Json["Actors"][UUID]["Location"];
        JSON Rotation = Json["Actors"][UUID]["Rotation"];
        JSON Scale = Json["Actors"][UUID]["Scale"];
        ObjectInfo->Location = FVector(Location[0].ToFloat(), Location[1].ToFloat(), Location[2].ToFloat());
        ObjectInfo->Rotation = FVector(Rotation[0].ToFloat(), Rotation[1].ToFloat(), Rotation[2].ToFloat());
        ObjectInfo->Scale = FVector(Scale[0].ToFloat(), Scale[1].ToFloat(), Scale[2].ToFloat());

        ObjectInfo->ObjectType = Json["Actors"][UUID]["Type"].ToString();
        
        index++;
    }
    return WorldInfo;
}

void JsonSaveHelper::SaveScene(const UWorldInfo& WorldInfo)
{
    if (WorldInfo.SceneName.empty())
        return;
    JSON Json;
    
    Json["Version"] = WorldInfo.Version;
    Json["NextUUID"] = UEngineStatics::NextUUID;
    Json["ActorCount"] = WorldInfo.ActorCount;
    Json["SceneName"] = WorldInfo.SceneName;

    for (uint32 i = 0; i < WorldInfo.ActorCount; i++)
    {
        UObjectInfo* ObjectInfo = WorldInfo.ObjctInfos[i];
        std::string Uuid = std::to_string(ObjectInfo->UUID);
        
        Json["Actors"][Uuid]["Location"].append(ObjectInfo->Location.X, ObjectInfo->Location.Y, ObjectInfo->Location.Z);
        Json["Actors"][Uuid]["Rotation"].append(ObjectInfo->Rotation.X, ObjectInfo->Rotation.Y, ObjectInfo->Rotation.Z);
        Json["Actors"][Uuid]["Scale"].append(ObjectInfo->Scale.X, ObjectInfo->Scale.Y, ObjectInfo->Scale.Z);
        Json["Actors"][Uuid]["Type"] = ObjectInfo->ObjectType;
    }
     
    std::ofstream Output(WorldInfo.SceneName + ".scene");
    
    if (Output.is_open())
    {
        Output << Json;
    }
}
