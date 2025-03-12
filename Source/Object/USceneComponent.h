#pragma once
#include "UObject.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Set.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Matrix.h"
#include "Object/ActorComponent/ActorComponent.h"

class USceneComponent : public UActorComponent
{
	friend class AActor;
	using Super = UActorComponent;
public:
	USceneComponent() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	// 과거 method입니다 GetComponentTransform()은 아래 다시 정의되어있습니다.
	/* 로컬 트랜스폼을 반환*/
	//FTransform GetComponentTransform() const { return RelativeTransform; }
	/* 월드 트랜스폼을 반환, 이걸로 렌더링한다*/
	//const FTransform GetWorldTransform();

	//void SetRelativeTransform(const FTransform& InTransform);

	void Pick(bool bPicked);
public:
	bool IsPicked() const { return bIsPicked; }


	///////////////////////////////////
	// 상속 부분
private:
	USceneComponent* AttachParent = nullptr;
	TSet<USceneComponent*> AttachChildren;
public:
	//void SetupAttachment(USceneComponent* InParent, bool bUpdateChildTransform = false); 
	// 부모의 월드 트랜스폼을 받아서 자신의 로컬 트랜스폼을 갱신
	//void ApplyParentWorldTransform(const FTransform& InTransform);

	bool AttachToComponent(USceneComponent* Parent); // 부모 변경
	bool DetachFromComponent(); // 부모로부터 떨어짐
	bool Remove(USceneComponent* Child);

	USceneComponent* GetAttachParent() const; // 현재 부모 반환
	void GetParentComponents(TSet<USceneComponent*>& Parents) const; // 현재 parent와 ancestor
	const USceneComponent* GetAttachmentRoot() const; // 부모로 거슬러가 root의 ancestor component를 리턴
	AActor* GetAttachmentRootActor() const;

	const TSet<USceneComponent*> GetAttachChildren() const; // 현재 직접 부착된 children 반환
	const void GetChildrenComponents(TSet< USceneComponent* >& Children) const; // 현재 부착된 children & descendent 리턴

	//void OnAttachmentChanged(); // 부모가 변경되었을때 호출
	//void OnChildAttached(USceneComponent* ChildComponent);
	//void OnChildDeleted(USceneComponent* ChildComponent);

private:
	//void SetAttachParent(USceneComponent* NewAttachParent);



	///////////////////////////////////
	// 변환 관련
private:
	FTransform RelativeTransform = FTransform();

public:
	void SetRelativeLocation(FVector NewRelativeLocation);
	void SetRelativeRotation(FQuat& NewRotation);
	void SetRelativeScale3D(FVector NewScale3D);
	void SetRelativeTransform(const FTransform& NewTransform);

	FVector GetRelativeLocation() const;
	FQuat GetRelativeRotation() const;
	FVector GetRelativeScale3D() const;
	FTransform GetRelativeTransform() const;
	const FTransform& GetComponentTransform() const; // 부모 SceneComponent의 영향을 받습니다. 렌더할땐 이 함수를 사용합니다.

	bool IsAttachedTo(const USceneComponent* TestComp) const; // ancestor중에 특정 component가 있는지 확인


protected:
	//USceneComponent* Parent = nullptr;
	//TSet<USceneComponent*> Children;
	// 이건 내 로컬 트랜스폼
	bool bCanEverTick = true;

	// debug
protected:
	bool bIsPicked = false;
};