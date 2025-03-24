#pragma once
#include "Core/EngineTypes.h"
#include "Object/UObject.h"
#include "Object/ActorComponent/ActorComponent.h"
#include "Core/Math/Transform.h"
#include "Core/Container/Set.h"
#include "Object/ObjectFactory.h"
#include "Object/USceneComponent.h"
#include "Object/PrimitiveComponent/TextComponent.h"
#include "Object/ObjectMacros.h"
#include "object/Cast.h"

class UWorld;

enum class RenderType
{
	DefaultType,
	BatchType,
	InstancedType,
};

class AActor : public UObject
{
	friend class FEditorManager;
	DECLARE_CLASS(AActor, UObject)
public:
	AActor();
	virtual ~AActor() override = default;

	void SetDepth(int InDepth)
	{
		Depth = InDepth;
	}

	int GetDepth() const
	{
		return Depth;
	}
public: 
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void LateTick (float DeltaTime); // 렌더 후 호출
	
	virtual void Destroyed();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	TSet<UActorComponent*>& GetComponents() { return Components; }

	UWorld* GetWorld() const { return World; }
	void SetWorld(UWorld* InWorld) { World = InWorld; }
	void SetUUIDTag();
	
	bool IsGizmoActor() const { return bIsGizmo; }
	bool IsCanPick() const { return bCanPick; }
	bool IsBatchActor() const { return bIsBatch; }

	void DontDestroy() { bDontDestroy = true; }
	bool IsDontDestroy() const { return bDontDestroy; }
	
	virtual void SetUUIDTextCanBeRendered(bool bRender);
	
private:
	virtual void Pick();
	virtual void UnPick();

	uint32 Depth;
	bool bIsPicked = false;
public:
	bool IsPicked() const { return bIsPicked; }

public:
	template<typename T>
		requires std::derived_from<T, UActorComponent>
	T* AddComponent()
	{
		T* ObjectInstance = FObjectFactory::ConstructObject<T>();
		Components.Add(ObjectInstance);
		ObjectInstance->SetOwner(this);


		// 만약 SceneComponent를 상속 받았다면
		//if constexpr (std::is_base_of_v<USceneComponent, T>)
		//{
		//	if (RootComponent == nullptr)
		//	{
		//		RootComponent = ObjectInstance;
		//	}
		//	else
		//	{
		//		ObjectInstance->SetupAttachment(RootComponent);
		//	}
		//}

		return ObjectInstance;
	}

	// delete
	template<typename T>
		requires std::derived_from<T, UActorComponent>
	void RemoveComponent(T* Object)
	{
		Components.Remove(Object);
	}

	FMatrix GetActorTransformMatrix();
	FTransform GetActorRelativeTransform() const;
	FMatrix GetActorRelativeTransformMatrix() const;

	void SetActorRelativeTransform(const FTransform& InTransform);
	bool CanEverTick() const { return bCanEverTick; }
	//virtual const char* GetTypeName();

	bool Destroy();

public:
	USceneComponent* GetRootComponent() const { return RootComponent; }
	void SetRootComponent(USceneComponent* InRootComponent) { RootComponent = InRootComponent; }
	void SetupAttachment(AActor* InParent);
	AActor* GetParent() const { return Parent; }

	template<typename T>
		requires std::derived_from<T, UActorComponent>
	T* GetComponentByClass() const
	{
		for (UActorComponent* Comp : Components)
		{
			if (T* Casted = Cast<T>(Comp)) 
			{
				return Casted;
			}
		}
		return nullptr;
	}


	
//public:
//	void SetColor(FVector4 InColor);

protected:
	bool bCanEverTick = true;
	bool bCanPick = true;
	bool bIsGizmo = false;
	bool bDontDestroy = false;
	USceneComponent* RootComponent = nullptr;
	UTextComponent* UUIDTextComponent = nullptr;

	bool bIsBatch = false;
	
	AActor* Parent = nullptr;
	TSet<AActor*> Children;
	TSet<UActorComponent*> Components;

	RenderType RenderType = RenderType::BatchType; //원래 DefaultType으로 해야하는데 임시방편으로 전부 배치로 하겠음
	
private:
	UWorld* World = nullptr;

public:
	
};


