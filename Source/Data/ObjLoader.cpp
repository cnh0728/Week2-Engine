#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Core/Math/Vector.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"
#include "Static/ResourceManager.h"

struct Face
{
    TArray<FVector> FaceIndices;
    std::string MaterialFile;
    std::string MaterialName;
};

bool ObjectLoader::LoadFromFile(const std::string& Filename)
{
    std::ifstream file(Filename);
    if (!file.is_open())
    {
        std::cerr << "파일을 열 수 없습니다!" << std::endl;
        return false;
    }

    std::string line;
    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FVector2> UVs;

    TMap<std::string, TArray<Face>> FaceGroup;
    TArray<Face> Faces;

    std::string CurrentObjectName = "";
    std::string CurrentMaterialFile = "";
    std::string CurrentMaterial = "";

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        line.erase(std::unique(line.begin(), line.end(),
            [](char a, char b) { return isspace(a) && isspace(b); }),
            line.end());

        TArray<std::string> Tokens = Split(line, ' ');
        size_t HashToken = Hash(Tokens[0]);

        if (HashToken == Hash("mtllib"))
        {
            CurrentMaterialFile = Tokens[1];
            UResourceManager::Get().LoadMtlFile(CurrentMaterialFile); // 머티리얼 파일 로딩
        }
        else if (HashToken == Hash("usemtl"))
        {
            CurrentMaterial = Tokens[1];
        }
        else if (HashToken == Hash("v"))
        {
            Vertices.Add({ std::stof(Tokens[1]), std::stof(Tokens[2]), std::stof(Tokens[3]) });
        }
        else if (HashToken == Hash("vn"))
        {
            Normals.Add({ std::stof(Tokens[1]), std::stof(Tokens[2]), std::stof(Tokens[3]) });
        }
        else if (HashToken == Hash("vt"))
        {
            UVs.Add({ std::stof(Tokens[1]), -std::stof(Tokens[2]) });
        }
        else if (HashToken == Hash("f"))
        {
            TArray<FVector> NewFaceIndex;
            for (int i = 1; i <= 3; i++)
            {
                TArray<std::string> FaceTokens = Split(Tokens[i], '/');
                if (FaceTokens.Num() == 2)
                    FaceTokens.Add("1");

                FVector NewIndex = { std::stof(FaceTokens[0]) - 1, std::stof(FaceTokens[1]) - 1, std::stof(FaceTokens[2]) - 1 };
                NewFaceIndex.Add(NewIndex);
            }

            if (Tokens.Num() == 5) // quad
            {
                TArray<std::string> FaceTokens = Split(Tokens[4], '/');
                FVector NewIndex = { std::stof(FaceTokens[0]) - 1, std::stof(FaceTokens[1]) - 1, std::stof(FaceTokens[2]) - 1 };
                NewFaceIndex.Add(NewFaceIndex[0]);
                NewFaceIndex.Add(NewFaceIndex[2]);
                NewFaceIndex.Add(NewIndex);
            }

            Face NewFace = { NewFaceIndex, CurrentMaterialFile, CurrentMaterial };
            Faces.Add(NewFace);
        }
        else if (HashToken == Hash("o") || HashToken == Hash("g"))
        {
            if (Faces.Num() != 0)
            {
                FaceGroup[CurrentObjectName] = Faces;
                Faces.Empty();
            }

            CurrentObjectName = Tokens[1];
        }
    }

    if (Faces.Num() != 0)
    {
        FaceGroup[CurrentObjectName] = Faces;
        Faces.Empty();
    }

   // 데이터 저장~
    TMap<std::string, uint32> VertexMap;
    TArray<FSubMeshData> SubMeshes;

    for (auto& Pair : FaceGroup)
    {
        TArray<Face>& FacesInGroup = Pair.Value;

        TMap<std::string, FSubMeshData> SubMeshPerMaterial;

        for (const Face& CurrentFace : FacesInGroup)
        {
            FSubMeshData& SubMesh = SubMeshPerMaterial[CurrentFace.MaterialName];
            SubMesh.MaterialName = CurrentFace.MaterialName;

            for (const FVector& FaceIndex : CurrentFace.FaceIndices)
            {
                FVertexSimple NewVertex;
                NewVertex.SetPos(Vertices[FaceIndex.X]);
                NewVertex.SetUV(UVs[FaceIndex.Y]);
                NewVertex.SetNormal(Normals[FaceIndex.Z]);
                NewVertex.SetVertexColor({ 1.f, 1.f, 1.f, 1.f });

                std::string VertexKey = std::to_string(FaceIndex.X) + " " + std::to_string(FaceIndex.Y) + " " + std::to_string(FaceIndex.Z);

                if (VertexMap.Contains(VertexKey))
                {
                    SubMesh.Indices.Add(VertexMap[VertexKey]);
                }
                else
                {
                    uint32 Index = SubMesh.Vertices.Num();
                    SubMesh.Vertices.Add(NewVertex);
                    VertexMap[VertexKey] = Index;
                    SubMesh.Indices.Add(Index);
                }
            }
        }

        for (auto& SubMeshPair : SubMeshPerMaterial)
        {
            SubMeshes.Add(SubMeshPair.Value);
        }
    }

    UResourceManager::Get().SetMeshData(Filename, SubMeshes);
    return true;
}

size_t ObjectLoader::Hash(std::string Str)
{
    size_t Hash = 0;
    for (size_t i = 0; i < Str.length(); i++)
    {
        Hash = 65599 * Hash + Str[i];
    }
    return Hash ^ (Hash >> 16);
}

TArray<std::string> ObjectLoader::Split(const std::string& str, char delim) {
    std::istringstream iss(str);
    TArray<std::string> result;
    std::string token;
    while (std::getline(iss, token, delim)) {
        if (token.empty())
            result.Add("1");
        else
            result.Add(token);
    }
    return result;
}
