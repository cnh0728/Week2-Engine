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

FMatrix USceneComponent::GetComponentTransformMatrix()
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

bool USceneComponent::SetupAttachment(USceneComponent* InParent)
{
	// InParent 유효성 조사
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
	if (InParent != nullptr)
	{
		// InParent 의 조상중에 this가 있는지 확인
		TArray<USceneComponent*> AncestorsOfInParent;
		InParent->GetParentComponents(AncestorsOfInParent);
		if (AncestorsOfInParent.Find(this) != -1)
		{
			// InParent의 조상중에 this가 있음 -> 순환
			UE_LOG("USceneComponent::AttachToComponent::Cannot attach to descendant\n");
			return false;
		}
	}

	// 현재 AttachParent의 AttachChildren 목록에서 this 제거
	if (Parent)
	{
		if (Parent->Children.Remove(this)) {
			UE_LOG("USceneComponent::AttachToComponent::Unknown error\n"); // 부모에 자신이 등록되어있지 않음
			return false;
		}
	}

	Parent = InParent; // 부모로 추가
	if (InParent) // 자식으로 추가
	{
		InParent->Children.Add(this);
	}
	return true;
}


bool USceneComponent::DetachFromComponent()
{
	if (Parent != nullptr)
	{
		TSet<USceneComponent*> Siblings = Parent->Children;
		TSet<USceneComponent*>::Iterator Iter = Siblings.Find(this);
		if (Iter == Siblings.end())
		{
			UE_LOG("USceneComponent::Remove::Unknown Error\n"); // this -> parent이지만, parent -> this는 아님. 발생시 코드 수정필요
			return false;
		}
		Parent->Children.Remove(this);
		this->Parent = nullptr;
	}
	else
	{
		UE_LOG("USceneComponent::DetachFromComponent::Tried to detach from nullptr\n");
		return false;
	}
}

void USceneComponent::GetParentComponents(TArray<USceneComponent*>& Parents) const
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


// children 만 리턴합니다.
const TSet<USceneComponent*> USceneComponent::GetAttachChildren() const
{
	return Children;
}


// 아래에 있는 모든 descendant를 얻습니다.
void USceneComponent::GetChildrenComponents(TSet<USceneComponent*>& Children) const
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


bool USceneComponent::IsAttachedTo(const USceneComponent* TestComp) const
{
	return Parent == TestComp;
}
