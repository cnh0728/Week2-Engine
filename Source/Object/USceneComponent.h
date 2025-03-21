#pragma once
#include "UObject.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Set.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Matrix.h"
#include "Object/ActorComponent/ActorComponent.h"
#include "Static/Enum.h"

#include "Object/ObjectMacros.h"

class USceneComponent : public UActorComponent
{
	friend class AActor;
	using Super = UActorComponent;
	DECLARE_CLASS(USceneComponent, UActorComponent)
public:
	USceneComponent() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	/* 로컬 트랜스폼을 반환*/
	FTransform GetRelativeTransform() const { return RelativeTransform; }
	FMatrix GetRelativeTransformMatrix() const;
	/* 월드 트랜스폼을 반환, 이걸로 렌더링한다*/
	const FTransform GetComponentTransform(); // !!! 사용금지 !!!
	virtual FMatrix GetComponentTransformMatrix();

	void SetRelativeTransform(const FTransform& InTransform);

	void Pick(bool bPicked);
public:
	bool IsPicked() const { return bIsPicked; }

public:
	bool SetupAttachment(USceneComponent* InParent);
	bool DetachFromComponent();

	FORCEINLINE USceneComponent* GetAttachParent() const { return Parent; }
	void GetParentComponents(TArray<USceneComponent*>& Parents) const;
	const USceneComponent* GetAttachmentRoot() const;
	AActor* GetAttachmentRootActor() const;

	const TSet<USceneComponent*> GetAttachChildren() const;
	void GetChildrenComponents(TSet<USceneComponent*>& Children) const;

	bool IsAttachedTo(const USceneComponent* TestComp) const;

protected:
// 추후 Getter Setter 추가 예정
	USceneComponent* Parent = nullptr;
	TSet<USceneComponent*> Children;
	// 이건 내 로컬 트랜스폼
	FTransform RelativeTransform = FTransform();
	bool bCanEverTick = true;

	// debug
protected:
	bool bIsPicked = false;

};