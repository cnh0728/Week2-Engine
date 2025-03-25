#include "ObjManager.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

struct Loader //로더로 바꿀때 사용
{
    TMap<std::string, TMap<std::string, Face>> Object;
    TArray<std::string> Materials;

    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FVector2> UVs;
};
#pragma pack(push, 1) //패딩 없음
struct BinaryHeader
{
    char Magic[4] = { 'O', 'B', 'J', 'B' };   //매직 넘버
    uint32_t Vertsion = 1;                  //파일 버전
    uint32_t SubMeshCount;                   //총 버텍스 수
    uint32_t MaterialCount;
};
#pragma pack(pop)
    

FObjManager::FObjManager()
{
    CheckExistAllDirectory();
}

void FObjManager::CheckExistAllDirectory()
{
    namespace fs = std::filesystem;
    
    if (!fs::exists(BinaryFileDir))
    {
        fs::create_directories(BinaryFileDir);    
    }

    if (!fs::exists(ObjFileDir))
    {
        fs::create_directories(ObjFileDir);
    }
}

bool FObjManager::LoadFromFile(const std::string& Filename)
{
    namespace fs = std::filesystem;

    bool bIsAbsolutePath = fs::path(Filename).is_absolute();
    std::string ObjPath = bIsAbsolutePath ? Filename : ObjFileDir + Filename + ObjFileExt;

  

    if (UResourceManager::Get().HasMeshData(Filename))
    {
        return true; // 이미 파싱된 경우
    }

    TArray<FSubMeshData> SubMeshes;

    namespace fs = std::filesystem;
    if (fs::exists(BinaryFileDir + Filename + BinaryFileExt))
    {
        TArray<std::string> MT;
        LoadFromBinary(SubMeshes, MT, Filename);

        for (auto& MaterialName : MT)
        {
            UResourceManager::Get().LoadMtlFile(MaterialName); // 머티리얼 파일 로딩
        }
        
        UResourceManager::Get().SetMeshData(Filename, SubMeshes);

        return true;
    }

    std::ifstream file(ObjPath);
    if (!file.is_open())
    {
        std::cerr << "파일을 열 수 없습니다: " << ObjPath << std::endl;
        return false;
    }

    std::string line;
    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FVector2> UVs;

    TMap<std::string, TArray<Face>> FaceGroup;
    TArray<Face> Faces;

    TArray<std::string> MaterialNames;
    std::string CurrentObjectName = "";
    std::string CurrentMaterialFile = "";
    std::string CurrentMaterial = "";

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#'){
            continue;
        }
        
        line.erase(std::unique(line.begin(), line.end(), 
          [](char a, char b) { return isspace(a) && isspace(b); }),
          line.end()); // 공백 2개이상 1개로 줄이는거
        
        TArray<std::string> Tokens = Split(line, ' ');
        size_t HashToken = Hash(Tokens[0]);

        if (HashToken == Hash("mtllib")) //일단 o, g생략
        {
            CurrentMaterialFile = Tokens[1];
            MaterialNames.Add(CurrentMaterialFile);
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

    if (Faces.Num() != 0) //마무리 저장
    {
        FaceGroup[CurrentObjectName] = Faces;
        Faces.Empty();
    }

   // 데이터 저장~
    TMap<std::string, uint32> VertexMap;

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
                FVertexPNCT NewVertex;
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
    
    SaveToBinary(SubMeshes, MaterialNames, Filename);
    UResourceManager::Get().SetMeshData(Filename, SubMeshes);
    return true;
}

size_t FObjManager::Hash(std::string Str)
{
    size_t Hash = 0;
    for (size_t i = 0; i < Str.length(); i++)
    {
        Hash = 65599 * Hash + Str[i];
    }
    return Hash ^ (Hash >> 16);
}

TArray<std::string> FObjManager::Split(const std::string& str, char delim) {
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

bool FObjManager::SaveToBinary(TArray<FSubMeshData>& SubMeshes, TArray<std::string>& Materials, const std::string& Filename)
{
    std::ofstream File(BinaryFileDir + Filename + BinaryFileExt, std::ios::binary);
    if (!File.is_open()) return false;

    // 헤더 작성
    BinaryHeader Header;
    Header.SubMeshCount = SubMeshes.Num();
    Header.MaterialCount = Materials.Num();
    File.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
    
    // SubMesh 데이터 쓰기
    for (auto& SubMesh : SubMeshes)
    {
        // Vertices 저장
        uint32_t vertexCount = SubMesh.Vertices.Num();
        File.write(reinterpret_cast<const char*>(&vertexCount), sizeof(uint32_t));
        File.write(reinterpret_cast<const char*>(SubMesh.Vertices.GetData()), vertexCount * sizeof(FVertexPNCT));

        // Indices 저장
        uint32_t indexCount = SubMesh.Indices.Num();
        File.write(reinterpret_cast<const char*>(&indexCount), sizeof(uint32_t));
        File.write(reinterpret_cast<const char*>(SubMesh.Indices.GetData()), indexCount * sizeof(uint32_t));

        // MaterialName 저장
        uint32_t nameLength = SubMesh.MaterialName.length();
        File.write(reinterpret_cast<const char*>(&nameLength), sizeof(uint32_t));
        File.write(SubMesh.MaterialName.c_str(), nameLength);
    }
 
    // Materials 저장
    for (const auto& Material : Materials)
    {
        uint32_t strLength = Material.length();
        File.write(reinterpret_cast<const char*>(&strLength), sizeof(uint32_t));
        File.write(Material.c_str(), strLength);
    }
    
    File.close();
    return true;
}

bool FObjManager::LoadFromBinary(TArray<FSubMeshData>& OutSubMeshes, TArray<std::string>& OutMaterials, const std::string& Filename)
{
    std::ifstream File(BinaryFileDir + Filename + BinaryFileExt, std::ios::binary);
    if (!File.is_open()) return false;

    BinaryHeader Header;
    File.read(reinterpret_cast<char*>(&Header), sizeof(Header));

    // SubMesh 데이터 읽기
    OutSubMeshes.Resize(Header.SubMeshCount);
    for (auto& SubMesh : OutSubMeshes)
    {
        // Vertices 읽기
        uint32_t vertexCount;
        File.read(reinterpret_cast<char*>(&vertexCount), sizeof(uint32_t));
        SubMesh.Vertices.Resize(vertexCount);
        File.read(reinterpret_cast<char*>(SubMesh.Vertices.GetData()), vertexCount * sizeof(FVertexPNCT));

        // Indices 읽기
        uint32_t indexCount;
        File.read(reinterpret_cast<char*>(&indexCount), sizeof(uint32_t));
        SubMesh.Indices.Resize(indexCount);
        File.read(reinterpret_cast<char*>(SubMesh.Indices.GetData()), indexCount * sizeof(uint32_t));

        // MaterialName 읽기
        uint32_t nameLength;
        File.read(reinterpret_cast<char*>(&nameLength), sizeof(uint32_t));
        SubMesh.MaterialName.resize(nameLength);
        File.read(&SubMesh.MaterialName[0], nameLength);
    }

    // Materials 읽기
    OutMaterials.Resize(Header.MaterialCount);
    for (auto& Material : OutMaterials)
    {
        uint32_t strLength;
        File.read(reinterpret_cast<char*>(&strLength), sizeof(uint32_t));
        Material.resize(strLength);
        File.read(&Material[0], strLength);
    }
    
    File.close();
    return true;
}

