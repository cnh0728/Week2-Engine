#pragma once
#include "UObject.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Set.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Matrix.h"
#include "Object/ActorComponent/ActorComponent.h"

// https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Engine/EDetachmentRule
enum class EAttachmentRule
{
	KeepRelative, // relative 유지
	KeepWorld, // world transform을 relative transform으로 변경
};

typedef struct FAttachmentTransformRules
{
	EAttachmentRule LocationRule;
	EAttachmentRule RotationRule;
	EAttachmentRule ScaleRule;

	FAttachmentTransformRules() : LocationRule(EAttachmentRule::KeepRelative), RotationRule(EAttachmentRule::KeepRelative), ScaleRule(EAttachmentRule::KeepRelative) {}

	FAttachmentTransformRules(EAttachmentRule InLocationRule,
		EAttachmentRule InRotationRule,
		EAttachmentRule InScaleRule) : LocationRule(InLocationRule), RotationRule(InRotationRule), ScaleRule(InScaleRule) {
	}

	FAttachmentTransformRules operator=(bool bValue) {
		if (bValue == false) {
			LocationRule = EAttachmentRule::KeepRelative;
			RotationRule = EAttachmentRule::KeepRelative;
			ScaleRule = EAttachmentRule::KeepRelative;
		}
		else
		{
			LocationRule = EAttachmentRule::KeepWorld;
			RotationRule = EAttachmentRule::KeepWorld;
			ScaleRule = EAttachmentRule::KeepWorld;
		}
	}
};


enum class EDetachmentRule
{
	KeepRelative, // relative 유지
	KeepWorld, // world transform을 relative transform으로 변경
};

typedef struct FDetachmentTransformRules
{
	EDetachmentRule LocationRule;
	EDetachmentRule RotationRule;
	EDetachmentRule ScaleRule;

	FDetachmentTransformRules() : LocationRule(EDetachmentRule::KeepRelative), RotationRule(EDetachmentRule::KeepRelative), ScaleRule(EDetachmentRule::KeepRelative) {}

	FDetachmentTransformRules(EDetachmentRule InLocationRule,
		EDetachmentRule InRotationRule,
		EDetachmentRule InScaleRule) : LocationRule(InLocationRule), RotationRule(InRotationRule), ScaleRule(InScaleRule) {}

	FDetachmentTransformRules operator=(bool bValue) {
		if (bValue == false) {
			LocationRule = EDetachmentRule::KeepRelative;
			RotationRule = EDetachmentRule::KeepRelative;
			ScaleRule = EDetachmentRule::KeepRelative;
		}
		else
		{
			LocationRule = EDetachmentRule::KeepWorld;
			RotationRule = EDetachmentRule::KeepWorld;
			ScaleRule = EDetachmentRule::KeepWorld;
		}
	}
};



class USceneComponent : public UActorComponent
{
	friend class AActor;
	using Super = UActorComponent;
public:
	USceneComponent() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/////////////////////////////////////////////
	/// transform 관련
private:
	FTransform RelativeTransform = FTransform();
public:
	void SetRelativeTransform(const FTransform& InTransform);
	
	/* 로컬 트랜스폼을 반환*/
	FTransform GetRelativeTransform() const { return RelativeTransform; }
	FMatrix GetRelativeTransformMatrix() const;
	/* 월드 트랜스폼을 반환, 이걸로 렌더링한다*/
	const FTransform GetComponentTransform(); // !!! 사용금지 !!!
	const FMatrix GetComponentTransformMatrix();

	void Pick(bool bPicked);
public:
	bool IsPicked() const { return bIsPicked; }

// Hierarchy 관계
public:
	bool AttachToComponent(USceneComponent* InParent, const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules()); // 부모 변경
	bool DetachFromComponent(const FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules());
	
	USceneComponent* GetAttachParent() const;
	void GetParentComponents(TArray<USceneComponent*>& Parents) const;
	const USceneComponent* GetAttachmentRoot() const;
	AActor* GetAttachmentRootActor() const;


	const TSet<USceneComponent*> GetAttachChildren() const;

	void GetChildrenComponents(TSet<USceneComponent*>& Children) const;

	bool IsAttachedTo(const USceneComponent* TestComp) const;

private:
	USceneComponent* AttachParent = nullptr;
	TSet<USceneComponent*> AttachChildren;
	// 이건 내 로컬 트랜스폼
	bool bCanEverTick = true;

	// debug
protected:
	bool bIsPicked = false;
};