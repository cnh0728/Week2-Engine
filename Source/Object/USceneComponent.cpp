#include "USceneComponent.h"
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

// 내 월드 트랜스폼 반환
//const FTransform USceneComponent::GetWorldTransform()
//{
//	if (InParent)
//	{
//		// 부모가 있을 경우 부모 월드 * 내 로컬
//		FMatrix ParentWorld = InParent->GetWorldTransform().GetMatrix();
//		FMatrix MyLocal = RelativeTransform.GetMatrix();
//
//		FMatrix NewMatrix = MyLocal * ParentWorld;
//		return NewMatrix.GetTransform();
//	}
//
//	return RelativeTransform;
//}

//void USceneComponent::SetRelativeTransform(const FTransform& InTransform)
//{
//	// 내 로컬 트랜스폼 갱신
//	RelativeTransform = InTransform;
//	FVector Rot = RelativeTransform.GetRotation().GetEuler();
//
//}

void USceneComponent::Pick(bool bPicked)
{
	bIsPicked = bPicked;
	for (auto& Child : AttachChildren)
	{
		Child->Pick(bPicked);
	}
}

//void USceneComponent::SetupAttachment(USceneComponent* InParent, bool bUpdateChildTransform)
//{
//	if (InParent)
//	{
//		InParent = InParent;
//		InParent->Children.Add(this);
//		//ApplyParentWorldTransform(InParent->GetWorldTransform());
//	}
//	else
//	{
//		UE_LOG("Parent is nullptr");
//	}
//}

//void USceneComponent::ApplyParentWorldTransform(const FTransform& ParentWorldTransform)
//{
//	return;
//	FMatrix ParentWorld = ParentWorldTransform.GetMatrix();
//	FMatrix MyLocal = RelativeTransform.GetMatrix();
//
//	FMatrix NewMatrix = MyLocal * ParentWorld.Inverse();
//
//	// 내 로컬 트랜스폼 갱신
//	SetRelativeTransform(NewMatrix.GetTransform());
//}

//void USceneComponent::AttachToComponent(USceneComponent* Parent)
//{
//	// 현재 Parent의 child 목록에서 자신을 제거
//	if(!GEtAttach)
//}

// 모든 attach는 이걸로 구현
bool USceneComponent::AttachToComponent(USceneComponent* InParent)
{
	if (IsAttachedTo(InParent))
	{
		// 이미 붙어있음
		return true;
	}
	if (InParent == this)
	{
		UE_LOG("USceneComponent::AttachToComponent::Cannot attach to itself\n");
		return false;
	}

	// 먼저 떼야함
	if (GetAttachParent())
	{
		DetachFromComponent();
	}

	// this 의 조상중에 새로운 Parent가 있는지 확인
	// 이경우 순환하게됨
	TSet<USceneComponent*> AncestorsOfInParent;
	InParent->GetParentComponents(AncestorsOfInParent);
	if (AncestorsOfInParent.end() != std::find(AncestorsOfInParent.begin(), AncestorsOfInParent.end(), this))
	{
		UE_LOG("USceneComponent::AttachToComponent::Cannot attach to descendant\n");
	}
	// Parent의 child 목록에서 제거
	if (AttachParent)
	{
		//auto Iter = std::find(AttachParent->Children.begin(), AttachParent->Children.end(), this);
		TSet<USceneComponent*>::Iterator Iter = AttachParent->AttachChildren.Find(this);
		if (Iter != AttachParent->AttachChildren.end())
		{
			UE_LOG("USceneComponent::AttachToComponent::Unknown error\n"); // 부모에 자신이 등록되어있지 않음
			return false;
		}
		AttachParent->AttachChildren.Remove(this);
	}
	// Parent 등록
	AttachParent = InParent;
	if (InParent)
	{
		InParent->AttachChildren.Add(this);
	}
	return true;
}

bool USceneComponent::DetachFromComponent()
{
	if (AttachParent != nullptr)
	{
		if (AttachParent->Remove(this)) {
			return true;
		}
		else
		{
			UE_LOG("USceneComponent::DetachFromComponent::Unknown error\n");
			return false;
		}
	}
	else
	{
		UE_LOG("USceneComponent::DetachFromComponent::Tried to detach from nullptr\n"); 
		return false;
	}
}

// 나의 Child를 뗍니다.
bool USceneComponent::Remove(USceneComponent* Child)
{
	TSet<USceneComponent*>::Iterator Iter = AttachChildren.Find(Child);
	if (Iter == AttachChildren.end())
	{
		UE_LOG("USceneComponent::Remove::Tried to detach non-child\n");
		return false;
	}
	
	Child->AttachParent = nullptr;
	AttachChildren.Remove(Child);
	return true;
}

USceneComponent* USceneComponent::GetAttachParent() const
{
	return AttachParent;
}

void USceneComponent::GetParentComponents(TSet<USceneComponent*>& Parents) const
{
	Parents.Empty();
	
	// 언리얼  /Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h 참고함
	USceneComponent* ParentIterator = GetAttachParent();
	while (ParentIterator != nullptr)
	{
		Parents.Add(ParentIterator);
		ParentIterator = ParentIterator->GetAttachParent();
	}
}

const USceneComponent* USceneComponent::GetAttachmentRoot() const
{
	USceneComponent* ParentIterator = GetAttachParent();
	while (ParentIterator->GetAttachParent() != nullptr)
	{
		ParentIterator = ParentIterator->GetAttachParent();
	}
	return ParentIterator;
}


AActor* USceneComponent::GetAttachmentRootActor() const
{
	return GetAttachmentRoot()->Owner;
}

const TSet<USceneComponent*> USceneComponent::GetAttachChildren() const
{
	return AttachChildren;
}

const void USceneComponent::GetChildrenComponents(TSet<USceneComponent*>& Children) const
{
	Children.Empty();

	for (USceneComponent* Child : Children)
	{
		TSet<USceneComponent*> ChildComponents;
		Child->GetChildrenComponents(ChildComponents);
		for (USceneComponent* ChildComponent : ChildComponents)
		{
			Children.Add(ChildComponent);
		}
	}
}

//void USceneComponent::OnAttachmentChanged()
//{
//}
//
//void USceneComponent::OnChildAttached(USceneComponent* ChildComponent)
//{
//}
//
//void USceneComponent::OnChildDeleted(USceneComponent* ChildComponent)
//{
//}

bool USceneComponent::IsAttachedTo(const USceneComponent* TestComp) const
{
	return AttachParent == TestComp;
}

void USceneComponent::SetRelativeLocation(FVector NewRelativeLocation)
{
	RelativeTransform.SetPosition(NewRelativeLocation);
}

void USceneComponent::SetRelativeRotation(FQuat& NewRotation)
{
	RelativeTransform.SetRotation(NewRotation);
}

void USceneComponent::SetRelativeScale3D(FVector NewScale3D)
{
	RelativeTransform.SetScale(NewScale3D);
}

void USceneComponent::SetRelativeTransform(const FTransform& NewTransform)
{
	RelativeTransform = NewTransform;
}

FVector USceneComponent::GetRelativeLocation() const
{
	return RelativeTransform.GetPosition();
}

FQuat USceneComponent::GetRelativeRotation() const
{
	return RelativeTransform.GetRotation();
}

FVector USceneComponent::GetRelativeScale3D() const
{
	return RelativeTransform.GetScale();
}

FTransform USceneComponent::GetRelativeTransform() const
{
	return RelativeTransform;
}

const FTransform& USceneComponent::GetComponentTransform() const
{
	// root component일 경우
	if (AttachParent == nullptr)
	{
		return RelativeTransform;

	}
	else	
	{
		if (this->GetOwner()) {
			//auto rel = RelativeTransform.GetMatrix();
			//auto com = AttachParent->GetComponentTransform().GetMatrix();
			//auto res = rel * com;
			//return res.GetTransform();
			FTransform T = AttachParent->GetComponentTransform();
			return (RelativeTransform.GetMatrix() * T.GetMatrix()).GetTransform();
		}
		// owner의 rootcomponent가 scenecomponent이여야함
		// 이때의 Scenecomponent는 actor의 위치를 의미함
		// 만약 이 component가 속해있는 actor가 다른 actor의 child라면 ParentActor의 위치를 따라감
		// https://forums.unrealengine.com/t/why-do-people-use-scene-component-as-root-of-character-bp/270598
		// 하지만 지금은 Actor 사이의 Parent관계가 없음 (Owner 멤버변수가 없음)
		// https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/GameFramework/AActor 참고
		// 일단은 미구현
		//if (!ParentActor) // ParentActor가 없는 경우
		//{
		//	// 현재 SceneComponent가 최종 Transform
		//}
		//else
		//{
		//	//const FTransform ParentTransform = 
		//	//USceneComponent ParentActorRootComponent = ParentActor->GetRootComponent();
		//}


		assert(0);
	}
}
