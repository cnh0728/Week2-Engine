#pragma once
#include <string>
#include <vector>

#include "MaterialData.h"
#include "Core/Container/Array.h"

struct FVertexPNCT;

class FObjManager {
public:
    FObjManager();
    ~FObjManager() = default;
    void CheckExistAllDirectory();
    bool LoadFromFile(const std::string& Filename);
    size_t Hash(std::string Str);
    TArray<std::string> Split(const std::string& str, char delim);
    bool SaveToBinary(TArray<FSubMeshData>& SubMeshes, TArray<std::string>& Materials, const std::string& Filename);
    bool LoadFromBinary(TArray<FSubMeshData>& OutSubMeshes, TArray<std::string>& OutMaterials, const std::string& Filename);

private:
    std::string BinaryFileDir = "ObjectBinaries/";
    std::string BinaryFileExt = ".binary";
    std::string ObjFileDir = "Objects/";
    std::string ObjFileExt = ".obj";

};
