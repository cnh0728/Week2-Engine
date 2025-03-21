#include "World.h"
#include <cassert>
#include "Data/JsonSavehelper.h"

#include "Core/Container/Map.h"
#include "Core/Input/PlayerInput.h"
#include "Object/Actor/Camera.h"
#include <Object/Gizmo/GizmoHandle.h>

#include "Object/Actor/Cone.h"
#include "Object/Actor/Cube.h"
#include "Object/Actor/Cylinder.h"
#include "Object/Actor/Sphere.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"
#include "Object/PrimitiveComponent/TextComponent.h"
#include <Object/PrimitiveComponent/BillBoardComponent.h>

#include "Object/Actor/Texture.h"

void UWorld::BeginPlay()
{
	for (const auto& Actor : ActorsToSpawn)
	{
		Actor->BeginPlay();
	}
	ActorsToSpawn.Empty();
}

void UWorld::Tick(float DeltaTime)
{
	for (const auto& Actor : ActorsToSpawn)
	{
		Actor->BeginPlay();
	}
	ActorsToSpawn.Empty();

	const auto CopyActors = Actors;
	for (const auto& Actor : CopyActors)
	{
		if (Actor->CanEverTick())
		{
			Actor->Tick(DeltaTime);
		}
	}
}

void UWorld::LateTick(float DeltaTime)
{
	const auto CopyActors = Actors;
	for (const auto& Actor : CopyActors)
	{
		if (Actor->CanEverTick())
		{
			Actor->LateTick(DeltaTime);
		}
	}

	for (const auto& PendingActor : PendingDestroyActors)
	{
		// Engine에서 제거
		UEngine::Get().GObjects.Remove(PendingActor->GetUUID());
	}
	PendingDestroyActors.Empty();
}

void UWorld::Render(float DeltaTime)
{
	URenderer* Renderer = UEngine::Get().GetRenderer();

	if (Renderer == nullptr)
	{
		return;
	}

	
	ACamera* cam = FEditorManager::Get().GetCamera();
	Renderer->UpdateViewMatrix(cam->GetActorRelativeTransform());
	Renderer->UpdateProjectionMatrix(cam);
	
	RenderMainTexture(*Renderer, DeltaTime);
}

void UWorld::RenderPickingTexture(URenderer& Renderer)
{
	// Renderer.PreparePicking();
	// Renderer.PreparePickingShader();

	
	// for (auto& RenderComponent : RenderComponents)
	// {
	// 	if (RenderComponent->GetOwner()->GetDepth() > 0)
	// 	{
	// 		continue;
	// 	}
	// 	uint32 UUID = RenderComponent->GetUUID();
	// 	RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
	// 	RenderComponent->Render();
	// }
	//
	// Renderer.PrepareZIgnore();
	// for (auto& RenderComponent: ZIgnoreRenderComponents)
	// {
	// 	uint32 UUID = RenderComponent->GetUUID();
	// 	RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
	// 	uint32 depth = RenderComponent->GetOwner()->GetDepth();
	// 	RenderComponent->Render();
	// }
}

void UWorld::RenderMainTexture(URenderer& Renderer, float DeltaTime)
{
	Renderer.PrepareMain();

	Renderer.RenderBatch();

//prepare texture하고 loadtexture 하고 텍스쳐렌더 돌면서
	
	// Renderer.LoadTexture(TODO, "Texture.png");

	Renderer.PrepareMainShader();
	
	for (auto& RenderComponent : RenderComponents)
	{
		if (!FEditorManager::Get().IsShowFlagSet((EEngineShowFlags::SF_Primitives)))
		{
			if (RenderComponent->IsCanPick()) continue;
		}
		{
			//분기해주고 로드텍스쳐까지
			if (RenderComponent->IsA(UTextureComp::StaticClass()))
			{
				UTextureComp* TextureComponent = dynamic_cast<UTextureComp*>(RenderComponent);
				Renderer.PrepareTextureResource(TextureComponent->GetTextureResource());
			}
			RenderComponent->Render();
		}
	}

	Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		RenderComponent->Render();
	}

	for (auto& RenderComponent : TextRenderComponents)
	{
		if (!FEditorManager::Get().IsShowFlagSet(EEngineShowFlags::SF_BillboardText))
			continue;

		if (UTextComponent* TextComponent = dynamic_cast<UTextComponent*>(RenderComponent))
			{
				TextComponent->RenderText(Renderer, TextComponent->GetText(),
				TextComponent->GetComponentTransformMatrix().GetTranslation(),
				TextComponent->GetTextSize());
			}
	}
	
	Renderer.RenderParticle(DeltaTime);

	for (auto& RenderComponent : BillBoardRenderComponents)
	{
		UBillBoardComponent* BillBoardComponent = Cast<UBillBoardComponent>(RenderComponent);
		Renderer.RenderTexture(BillBoardComponent->GetComponentTransformMatrix().GetTranslation());
	}

}

void UWorld::DisplayPickingTexture(URenderer& Renderer)
{
	Renderer.RenderPickingTexture();
}

void UWorld::ClearWorld()
{
	TArray CopyActors = Actors;
	for (AActor* Actor : CopyActors)
	{
		if (Actor->IsDontDestroy() == false)
		{
			DestroyActor(Actor);
		}
	}

	ACamera* Camera = FEditorManager::Get().GetCamera();
	if (Camera)
	{
		Camera->SetActorRelativeTransform(Camera->GetSpawnTransform());
	}

	UE_LOG("Clear World");
}


bool UWorld::DestroyActor(AActor* InActor)
{
	// 나중에 Destroy가 실패할 일이 있다면 return false; 하기
	assert(InActor);

	if (PendingDestroyActors.Find(InActor) != -1)
	{
		return true;
	}

	// 삭제될 때 Destroyed 호출
	InActor->Destroyed();
	
	// World에서 제거
	Actors.Remove(InActor);

	//Render할 목록에서 제거
	// UEngine::Get().GetRenderer()->ReleaseVertexBuffer(InActor->GetUUID());
	
	// 제거 대기열에 추가
	PendingDestroyActors.Add(InActor);
	return true;
}

void UWorld::SaveWorld(const std::string& SceneName)
{
	const UWorldInfo& WorldInfo = GetWorldInfo();
	JsonSaveHelper::SaveScene(WorldInfo, SceneName);
}

void UWorld::AddZIgnoreComponent(UPrimitiveComponent* InComponent)
{
	ZIgnoreRenderComponents.Add(InComponent);
	InComponent->SetIsOrthoGraphic(true);
}

void UWorld::AddTextComponent(UPrimitiveComponent* InComponent)
{
	TextRenderComponents.Add(InComponent);
}

void UWorld::AddBillBoardComponent(UPrimitiveComponent* InComponent)
{
	BillBoardRenderComponents.Add(InComponent);
}

void UWorld::LoadWorld(const char* SceneName)
{
	if (SceneName == nullptr || strcmp(SceneName, "") == 0){
		return;
	}

	if (ActorFactoryMap.empty())
	{
		ActorFactoryMap = {
			{ "AActor",    [this]() { return SpawnActor<AActor>(); } },
			{ "ASphere",   [this]() { return SpawnActor<ASphere>(); } },
			{ "ACube",     [this]() { return SpawnActor<ACube>(); } },
			{ "AArrow",    [this]() { return SpawnActor<AArrow>(); } },
			{ "ACylinder", [this]() { return SpawnActor<ACylinder>(); } },
			{ "ACone",     [this]() { return SpawnActor<ACone>(); } },
			{ "ATexture",  [this]() { return SpawnActor<ATexture>(); } }
		};
	}
	
	UWorldInfo* WorldInfo = JsonSaveHelper::LoadScene(SceneName);
	if (WorldInfo == nullptr) return;

	ClearWorld();

	Version = WorldInfo->Version;
	this->SceneName = WorldInfo->SceneName;
	uint32 ActorCount = WorldInfo->ActorCount;

	// Type 확인
	for (uint32 i = 0; i < ActorCount; i++)
	{
		UObjectInfo* ObjectInfo = WorldInfo->ObjctInfos[i];
		FTransform Transform = FTransform(ObjectInfo->Location, ObjectInfo->Rotation, ObjectInfo->Scale);

		AActor* Actor = nullptr;

		if (ObjectInfo->ObjectType == "ACamera")
		{
			ACamera* Camera = FEditorManager::Get().GetCamera();
			if (Camera == nullptr) //지금 없으면 스폰 있으면 스폰할필요 없음
			{
				Camera = SpawnActor<ACamera>();
				FEditorManager::Get().SetCamera(Camera);
			}
			Actor = Camera;
		}else
		{
			static_cast<void*>(ActorFactoryMap[ObjectInfo->ObjectType]());
		}
		if (Actor)
		{
			Actor->SetActorRelativeTransform(Transform);
		}
	}
}

UWorldInfo UWorld::GetWorldInfo() const
{
	UWorldInfo WorldInfo;
	WorldInfo.ActorCount = Actors.Num();
	WorldInfo.ObjctInfos = new UObjectInfo*[WorldInfo.ActorCount];
	WorldInfo.SceneName = *SceneName;
	WorldInfo.Version = 1;
	uint32 i = 0;
	for (auto& actor : Actors)
	{
		if (Cast<ACamera>(actor) == nullptr) //카메라빼고 전부
		{
			if (actor->IsDontDestroy())
			{
				WorldInfo.ActorCount--;
				continue;
			}
		}
		
		WorldInfo.ObjctInfos[i] = new UObjectInfo();
		const FTransform& Transform = actor->GetActorRelativeTransform();
		WorldInfo.ObjctInfos[i]->Location = Transform.GetPosition();
		WorldInfo.ObjctInfos[i]->Rotation = Transform.GetRotation();
		WorldInfo.ObjctInfos[i]->Scale = Transform.GetScale();
		WorldInfo.ObjctInfos[i]->ObjectType = actor->GetTypeName();

		WorldInfo.ObjctInfos[i]->UUID = actor->GetUUID();
		i++;
	}
	return WorldInfo;
}