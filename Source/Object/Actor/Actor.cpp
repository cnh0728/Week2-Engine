#include "Actor.h"
#include "Object/USceneComponent.h"
#include "Debug/DebugConsole.h"
#include "Object/World/World.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"
#include "Object/PrimitiveComponent/TextComponent.h"
#include "Object/PrimitiveComponent/BillBoardComponent.h"
#include "Object/Cast.h"
#include "Object/UObjectArray.h"

// TMap<uint32, std::shared_ptr<UObject>> GObjects;

AActor::AActor() : Depth{ 0 }
{
	RootComponent = AddComponent<USceneComponent>();
}

void AActor::BeginPlay()
{
	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	
	for (auto& Component : Components) 
	{
		Component->BeginPlay();

		//if (UPrimitiveComponent* PrimitiveComponent = dynamic_cast<UPrimitiveComponent*>(Component))
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			Topology = PrimitiveComponent->GetTopology(PrimitiveComponent->GetType());
			PrimitiveComponent->RegisterComponentWithWorld(World);
		}
	}

	if (IsBatchActor()) //액터별로 관리해야하는데 지금 토폴로지별이라 임시
	{
		URenderer* Renderer = UEngine::Get().GetRenderer();
		Renderer->ResizeBatchVertexBuffer(Topology);
	}

}

void AActor::Tick(float DeltaTime)
{
	for (auto& Component : Components)
	{
		if (Component->CanEverTick())
		{
			Component->Tick(DeltaTime);
		}
	}
}

void AActor::LateTick(float DeltaTime)
{
}

void AActor::Destroyed()
{
	EndPlay(EEndPlayReason::Destroyed);
}


void AActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (auto& Component : Components)
	{		
		Component->EndPlay(EndPlayReason);
		//if (const auto PrimitiveComp = dynamic_cast<UPrimitiveComponent*>(Component))
		if (const auto PrimitiveComp = Cast<UPrimitiveComponent>(Component))
		{
			if (World->ContainsZIgnoreComponent(PrimitiveComp))
			{
				World->RemoveZIgnoreComponent(PrimitiveComp);
			}
			
			if (const auto TextComp = Cast<UTextComponent>(PrimitiveComp)) {
				World->RemoveTextComponent(PrimitiveComp);
			}

			if (const auto BillComp = Cast<UBillBoardComponent>(PrimitiveComp)) {
				World->RemoveTextComponent(PrimitiveComp);
			}

			GetWorld()->RemoveRenderComponent(PrimitiveComp);
		}
		if (FEditorManager::Get().GetSelectedActor() == this)
		{
			FEditorManager::Get().SelectComponent(nullptr);
		}
		GObjects.Remove(Component->GetUUID());
	}
	Components.Empty();
}

void AActor::Pick()
{
	if (RootComponent)
	{
		bIsPicked = true;
		RootComponent->Pick(true);
	}
}

void AActor::UnPick()
{
	if (RootComponent)
	{
		bIsPicked = false;
		RootComponent->Pick(false);
	}	
}

FMatrix AActor::GetActorTransformMatrix()
{
	if (Parent)
	{
		// 부모가 있을 경우 부모 월드 * 내 로컬
		FMatrix ParentWorld = Parent->GetActorTransformMatrix();
		FMatrix MyLocal = GetActorRelativeTransform().GetMatrix();

		FMatrix NewMatrix = MyLocal * ParentWorld;
		return NewMatrix;
	}

	return GetActorRelativeTransformMatrix();
}

// actor의 root component는 actor의 (아직은) 월드상 좌표입니다.
// @TODO : actor가 parent를 갖게되면 actor의 parent의 transform을 따라가야합니다.
FTransform AActor::GetActorRelativeTransform() const
{
	return RootComponent != nullptr ? RootComponent->GetRelativeTransform() : FTransform();
}

FMatrix AActor::GetActorRelativeTransformMatrix() const
{
	return this->GetActorRelativeTransform().GetMatrix();
}

// actor의 root component는 actor의 (아직은) 월드상 좌표입니다.
void AActor::SetActorRelativeTransform(const FTransform& InTransform)
{
	// InTransform은 월드 기준임
	if (RootComponent)
	{
		RootComponent->SetRelativeTransform(InTransform);
	}
	else
	{
		UE_LOG("RootComponent is nullptr");
	}
}

//const char* AActor::GetTypeName()
//{
//	return "Actor";
//}

bool AActor::Destroy()
{
	return GetWorld()->DestroyActor(this);
}

//void AActor::SetColor(FVector4 InColor)
//{
//	if (RootComponent == nullptr)
//	{
//		return;
//	}
//
//	UPrimitiveComponent* RootPrimitive = dynamic_cast<UPrimitiveComponent*>(RootComponent);
//	if (RootPrimitive)
//	{
//		RootPrimitive->SetColor(InColor);
//	}
//
//	for (auto& Component : Components)
//	{
//		UPrimitiveComponent* PrimitiveComponent = dynamic_cast<UPrimitiveComponent*>(Component);
//		if (PrimitiveComponent)
//		{
//			PrimitiveComponent->SetColor(InColor);
//		}
//	}
//}


void AActor::SetUUIDTag()
{
	//Super::BeginPlay 보다 빨리 실행해야 한다. -> 늦게하면 component에 추가가 안됨
	FString UUIDTag("UUID: ");
	UUIDTag += FString::FromInt(GetUUID());
	
	UUIDTextComponent = AddComponent<UTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);
	UUIDTextComponent->SetRelativeTransform(FTransform(FVector(0.0f, 0.9f, 0.9f), FVector(), FVector().One()));
	UUIDTextComponent->SetText(UUIDTag);

	UUIDTextComponent->SetIsDefaultRendered(false);
	UEngine::Get().GetWorld()->AddTextComponent(UUIDTextComponent);
	
	UUIDTextComponent->bIsUUIDText = true;
}

void AActor::SetUUIDTextCanBeRendered(bool bRender) {
	if (UUIDTextComponent)
	{
		UUIDTextComponent->SetIsDefaultRendered(bRender);
	}
}

void AActor::SetupAttachment(AActor* InParent)
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
