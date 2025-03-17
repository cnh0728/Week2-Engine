#include "USceneComponent.h"

#include "Actor/Actor.h"
#include "Debug/DebugConsole.h"
#include "PrimitiveComponent/UPrimitiveComponent.h"

void USceneComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USceneComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FMatrix USceneComponent::GetRelativeTransformMatrix() const
{
	return this->RelativeTransform.GetMatrix();
}

// 내 월드 트랜스폼 반환
// !!!!  사용금지 !!!!
// world transform이 필요하면 GetComponentTransformMatrix 를 이용하세요
// @TODO: iteration으로 중간에 FMatrix -> FTransform 없이 한번에 가져와야함
// Matrix계산으로 recursive하게 계산하고, 마지막에만 FTransform으로 해야함
// 따라서 Internal (helper) 함수 필요.
const FTransform USceneComponent::GetComponentTransform()
{
	if (Parent)
	{
		// 부모가 있을 경우 부모 월드 * 내 로컬
		FMatrix ParentWorld = Parent->GetComponentTransform().GetMatrix();
		FMatrix MyLocal = RelativeTransform.GetMatrix();

		FMatrix NewMatrix = MyLocal * ParentWorld;
		return NewMatrix.GetTransform();
	}

	return RelativeTransform;
}

const FMatrix USceneComponent::GetComponentTransformMatrix()
{
	if (Parent)
	{
		// 부모가 있을 경우 부모 월드 * 내 로컬
		FMatrix ParentWorld = Parent->GetComponentTransformMatrix();
		FMatrix MyLocal = GetRelativeTransformMatrix();

		FMatrix NewMatrix = MyLocal * ParentWorld;
		return NewMatrix;
	}

	//여기까지 왔으면 부모가 없는애고(루트컴포넌트) 액터의 부모가 있으면 부모월드 * 내 로컬
	if (AActor* ParentActor = GetOwner()->GetParent())
	{
		FMatrix ParentActorWorld = ParentActor->GetActorTransformMatrix();
		FMatrix MyLocal = GetRelativeTransformMatrix();

		FMatrix NewMatrix = MyLocal * ParentActorWorld;
		return NewMatrix;
	}

	//컴포넌트도 최상단이고 액터도 최상단이면 이거
	return GetRelativeTransformMatrix();
}

void USceneComponent::SetRelativeTransform(const FTransform& InTransform)
{
	// 내 로컬 트랜스폼 갱신
	RelativeTransform = InTransform;
}

void USceneComponent::Pick(bool bPicked)
{
	bIsPicked = bPicked;
	for (auto& Child : Children)
	{
		Child->Pick(bPicked);
	}
}

void USceneComponent::SetupAttachment(USceneComponent* InParent)
{
	if (InParent)
	{
		Parent = InParent;
		InParent->Children.Add(this);
	}
	else
	{
		UE_LOG("Parent is nullptr");
	}
}

void USceneComponent::SetColor(FVector4 Color)
{
	this->Color = Color;
}

