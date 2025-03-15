#pragma once
#include "Data/JsonSavehelper.h"
#include "Core/Engine.h"
#include "Core/Container/Array.h"
#include "Core/Container/Set.h"
#include "Core/Container/String.h"
#include "Object/UObject.h"
#include "Debug/DebugConsole.h"
#include "Object/ObjectFactory.h"

#include "Object/Actor/Arrow.h"
#include "Object/Actor/Picker.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class AActor;


class UWorld :public UObject
{
public:
	UWorld() = default;
	virtual ~UWorld() = default;

public:
	void BeginPlay();
	void Tick(float DeltaTime);
	void LateTick(float DeltaTime);

	template <typename T>
		requires std::derived_from<T, AActor>
	T* SpawnActor();
  
	bool DestroyActor(AActor* InActor);
	
	void Render();
	void RenderPickingTexture(URenderer& Renderer);
	void DisplayPickingTexture(URenderer& Renderer);
	void RenderMainTexture(URenderer& Renderer);

	void ClearWorld();
	void LoadWorld(const char* SceneName);
	void SaveWorld();

	void AddZIgnoreComponent(UPrimitiveComponent* InComponent);
	void RemoveZIgnoreComponent(UPrimitiveComponent* InComponent) {ZIgnoreRenderComponents.Remove(InComponent); }
	bool ContainsZIgnoreComponent(UPrimitiveComponent* InComponent) {return ZIgnoreRenderComponents.Find(InComponent) != -1; }
	
	// render
	void AddRenderComponent(class UPrimitiveComponent* Component) { RenderComponents.Add(Component); }
	void RemoveRenderComponent(class UPrimitiveComponent* Component) { RenderComponents.Remove(Component); }

	TSet<UPrimitiveComponent*> GetRenderComponents(){ return RenderComponents; }

	const TArray<AActor*>& GetActors() const { return Actors;  }
	
private:
	UWorldInfo GetWorldInfo() const;

public:
	FString SceneName;
	uint32 Version = 1;
	
protected:
	TArray<AActor*> Actors;
	TArray<UPrimitiveComponent*> ZIgnoreRenderComponents;
	TArray<AActor*> ActorsToSpawn;
	TArray<AActor*> PendingDestroyActors; // TODO: 추후에 TQueue로 변경
	TSet<UPrimitiveComponent*> RenderComponents;
};

template <typename T>
	requires std::derived_from<T, AActor>
T* UWorld::SpawnActor()
{
	T* Actor = FObjectFactory::ConstructObject<T>();
	
	if (UWorld* World = UEngine::Get().GetWorld())
	{
		Actor->SetWorld(World);
		Actors.Add(Actor);
		ActorsToSpawn.Add(Actor);
		return Actor;
	}

	UE_LOG("Actor Construction Failed. World is nullptr");
	return nullptr;
}