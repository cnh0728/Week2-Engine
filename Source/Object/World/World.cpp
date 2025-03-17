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

void UWorld::BeginPlay()
{
	for (const auto& Actor : Actors)
	{
		Actor->BeginPlay();
	}
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

void UWorld::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();

	if (Renderer == nullptr)
	{
		return;
	}

	ACamera* cam = FEditorManager::Get().GetCamera();
	Renderer->UpdateViewMatrix(cam->GetActorRelativeTransform());
	Renderer->UpdateProjectionMatrix(cam);

	Renderer->UpdateConstant();
	// if (APlayerInput::Get().GetMouseDown(false))
	// {
	// 	RenderPickingTexture(*Renderer);
	// }
	
	RenderMainTexture(*Renderer);
	
	// DisplayPickingTexture(*Renderer);

}

void UWorld::RenderPickingTexture(URenderer& Renderer)
{
	Renderer.PreparePicking();
	Renderer.PreparePickingShader();

	Renderer.Render();
	
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

void UWorld::RenderMainTexture(URenderer& Renderer)
{
	Renderer.PrepareMain();
	Renderer.PrepareMainShader();

	//TODO: 현재 ZIgnoreRender 구현안돼있음 하고 구현하기

	//여기서 새로운 버텍스정보 만들어주기 때문에 그전에 비워주기
	Renderer.ClearVertex();
	for (auto& RenderComponent  : RenderComponents)
	{
		if (RenderComponent->GetCanBeRendered() == false)
		{
			continue;
		}
		if (!FEditorManager::Get().IsShowFlagSet(EEngineShowFlags::SF_Primitives))
			continue;

		// RenderComponent->UpdateConstantDepth(Renderer, depth);

		//여기서 위치정해줌 ㅇㅇ
		Renderer.AddVertices(RenderComponent);
	}
	
	Renderer.Render();

	for (auto& RenderComponent : RenderComponents)
	{
		if (UTextComponent* TextComponent = dynamic_cast<UTextComponent*>(RenderComponent))
		{
			if (UUUIDTextComponent* UUIDTextComponent = dynamic_cast<UUUIDTextComponent*>(TextComponent))
			{
				UUIDTextComponent->RenderText(Renderer, UUIDTextComponent->GetText(),
					UUIDTextComponent->GetComponentTransformMatrix().GetTranslation(),
					UUIDTextComponent->GetComponentTransformMatrix().GetScale() * UUIDTextComponent->GetUUIDTextSize());
			}
		}
	}

	//Renderer.RenderText();
	
	// Renderer.PrepareZIgnore();
	// for (auto& RenderComponent: ZIgnoreRenderComponents)
	// {
	// 	uint32 depth = RenderComponent->GetOwner()->GetDepth();
	// 	RenderComponent->Render();
	// }
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
		if (!Actor->IsCanPick())
		{
			DestroyActor(Actor);
		}
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

void UWorld::SaveWorld()
{
	const UWorldInfo& WorldInfo = GetWorldInfo();
	JsonSaveHelper::SaveScene(WorldInfo);
}

void UWorld::AddZIgnoreComponent(UPrimitiveComponent* InComponent)
{
	ZIgnoreRenderComponents.Add(InComponent);
	InComponent->SetIsOrthoGraphic(true);
}

void UWorld::LoadWorld(const char* SceneName)
{
	if (SceneName == nullptr || strcmp(SceneName, "") == 0){
		return;
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
		FTransform Transform = FTransform(ObjectInfo->Location, FQuat(), ObjectInfo->Scale);
		Transform.Rotate(ObjectInfo->Rotation);

		AActor* Actor = nullptr;
		
		if (ObjectInfo->ObjectType == "Actor")
		{
			Actor = SpawnActor<AActor>();
		}
		else if (ObjectInfo->ObjectType == "Sphere")
		{
			Actor = SpawnActor<ASphere>();
		}
		else if (ObjectInfo->ObjectType == "Cube")
		{
			Actor = SpawnActor<ACube>();
		}
		else if (ObjectInfo->ObjectType == "Arrow")
		{
			Actor = SpawnActor<AArrow>();
		}
		else if (ObjectInfo->ObjectType == "Cylinder")
		{
			Actor = SpawnActor<ACylinder>();
		}
		else if (ObjectInfo->ObjectType == "Cone")
		{
			Actor = SpawnActor<ACone>();
		}
		
		Actor->SetActorRelatvieTransform(Transform);
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
		if (actor->IsCanPick())
		{
			WorldInfo.ActorCount--;
			continue;
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