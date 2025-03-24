#pragma once
#include <string>
#include <vector>

#include "Core/Container/Array.h"



class ObjectLoader {
public:
    ObjectLoader();
    ~ObjectLoader() = default;
    void CheckExistAllDirectory();
    bool LoadFromFile(const std::string& Filename);
    size_t Hash(std::string Str);
    TArray<std::string> Split(const std::string& str, char delim);
    bool SaveToBinary(const TArray<FVertexSimple>& Vertices, TArray<uint32>& Indices, const std::string& Filename);
    bool LoadFromBinary(TArray<FVertexSimple>& OutVertices, TArray<uint32_t>& OutIndices, const std::string& Filename);

private:
    std::string BinaryFileDir = "ObjectBinaries/";
    std::string BinaryFileExt = ".binary";
    std::string ObjFileDir = "Objects/";
    std::string ObjFileExt = ".obj";

};
