#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Core/Math/Vector.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"

struct Face
{
    TArray<FVector> FaceIndices;
    std::string MatrerialFile;
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
    //Map으로 중복된 버텍스 나왔을 시 버텍스 새로 만들지 말고 기존에 있는 버텍스 인덱스로 유도하는 기능 추가해야함

    std::string CurrentObjectName = ""; 
    std::string CurrentGroupName = ""; //아직 사용 안함 추후 추가
    
    std::string CurrentMaterialFile = "";
    std::string CurrentMaterial = "";
    
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') //주석이랑 빈칸 넘기기
        {
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
        }
        else if (HashToken == Hash("usemtl"))
        {
            CurrentMaterial = Tokens[1];
        }
        else if (HashToken == Hash("v"))
        {
            Vertices.Add({std::stof(Tokens[1]), std::stof(Tokens[2]), std::stof(Tokens[3])});
        }
        else if (HashToken == Hash("vn"))
        {
            Normals.Add({std::stof(Tokens[1]), std::stof(Tokens[2]), std::stof(Tokens[3])});
        }
        else if (HashToken == Hash("vt"))
        {
            UVs.Add({std::stof(Tokens[1]), -std::stof(Tokens[2])});
        }
        else if (HashToken == Hash("f"))
        {
            TArray<FVector> NewFaceIndex;
            for (int i=1;i<=3; i++)
            {
                TArray<std::string> FaceTokens = Split(Tokens[i], '/');
                if (FaceTokens.Num() == 2) //마지막 생략일때만 예외가 있어서 그를 위한 예외처리
                {
                    FaceTokens.Add("1"); // 1빼줘서 0베이스로 만들어줄거기 때문에 1줌
                }
                FVector NewIndex = {std::stof(FaceTokens[0])-1, std::stof(FaceTokens[1])-1, std::stof(FaceTokens[2])-1};
                NewFaceIndex.Add(NewIndex);
            }
            
            if (Tokens.Num() == 5) // 4면, (5면은 걍 배제)
            {
                TArray<std::string> FaceTokens = Split(Tokens[4], '/');
                FVector NewIndex = {std::stof(FaceTokens[0])-1, std::stof(FaceTokens[1])-1, std::stof(FaceTokens[2])-1};
                NewFaceIndex.Add(NewFaceIndex[0]);  //0
                NewFaceIndex.Add(NewFaceIndex[2]);  //2
                NewFaceIndex.Add(NewIndex);         //3
            }
            
            Face NewFace = {NewFaceIndex, CurrentMaterial, CurrentMaterialFile};
            Faces.Add(NewFace);
        }
        else if (HashToken == Hash("o") || HashToken == Hash("g"))
        {
            if (Faces.Num() != 0) //그룹이 바뀌면 있던 faces저장
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
    
    /** 이 위까지 데이터 처리한거고 이걸 가지고 아래에서 원하는 곳에 넣어줌 */
    TMap<std::string, uint32> VertexMap;
    TArray<FVertexSimple> FinalVertices;
    TArray<uint32_t> FinalIndices;

    for (auto& Pair : FaceGroup)
    {
        std::string ObjectName = Pair.Key;
        TArray<Face>& ParseFace = Pair.Value;
        
        for (Face CurrentFace : ParseFace)
        {
            for (FVector FaceIndex : CurrentFace.FaceIndices)
            {
                FVertexSimple NewVertex;
                NewVertex.SetPos(Vertices[FaceIndex.X]);
                NewVertex.SetUV(UVs[FaceIndex.Y]);
                NewVertex.SetNormal(Normals[FaceIndex.Z]);

                NewVertex.SetVertexColor({1.f, 1.f, 1.f, 1.f});
      
                std::string VertexKey = std::to_string(FaceIndex.X) + " "
                                        + std::to_string(FaceIndex.Y) + " "
                                        + std::to_string(FaceIndex.Z);

                if (VertexMap.Contains(VertexKey)) //이미 있는 버텍스면 검사해서 있는거 사용
                {
                    FinalIndices.Add(VertexMap[VertexKey]);
                }else //없으면 새로 만들기
                {
                    uint32_t Index = FinalVertices.Num();
                    FinalVertices.Add(NewVertex);
                    VertexMap[VertexKey] = Index;
                    FinalIndices.Add(Index);
                }
            }
        }
    }

    OriginVertices[EPT_Custom] = FinalVertices;
    OriginIndices[EPT_Custom] = FinalIndices;
    
    return true;
}

size_t ObjectLoader::Hash(std::string Str)
{
    size_t Hash = 0;
    for (size_t i= 0;i < Str.length();i++)
    {
        Hash = 65599*Hash + Str[i];
    }
    return Hash ^ (Hash >> 16);
}

TArray<std::string> ObjectLoader::Split(const std::string& str, char delim) {
    std::istringstream iss(str);
    TArray<std::string> result;
    std::string token;
    while (std::getline(iss, token, delim)) {
        if (token.empty())
        {
            result.Add("1"); //인덱스는 1부터 시작이기때문에
        }else{
            result.Add(token);
        }
    }
    return result;
}