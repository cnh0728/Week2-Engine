
#include "StaticMeshComponent.h"
#include "core/Engine.h"
#include "Static/ResourceManager.h"

void UStaticMeshComponent::LoadFromObj(const std::string& path)
{
    // Obj 파일 로드
    UEngine::Get().GetObjLoader()->LoadFromFile(path);
    // ResourceManager에서 데이터 가져오기
    const TArray<FSubMeshData>* meshData = UResourceManager::Get().GetMeshData(path);
    if (!meshData) return;

    RenderUnits.Empty();

    for (const auto& sub : *meshData)
    {
        FStaticMesh unit;
        unit.Vertices = sub.Vertices;
        unit.Indices = sub.Indices;
        unit.Material = UResourceManager::Get().GetMaterial(sub.MaterialName);
        RenderUnits.Add(unit);
    }
}
