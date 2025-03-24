#include "GizmoHandle.h"

#include "Object/Actor/Camera.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/PrimitiveComponent/BillBoardComponent.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"
#include "Core/Input/PlayerInput.h"
#include "Object/Cast.h"
#include "Object/UObjectArray.h"

TMap<uint32, std::shared_ptr<UObject>> GObjects;

AGizmoHandle::AGizmoHandle()
{
	DontDestroy();
	bIsGizmo = true;
	const float GizmoSize = 0.1f;
	const float GizmoSizeMultiplier = 1 / GizmoSize;
	// gizmo 중심을 잡기 위한 가상의 component
	USceneComponent* GizmoCenter = AddComponent<USceneComponent>();
	SetRootComponent(GizmoCenter);

	GizmoCube = AddComponent<UCubeComp>();
	GizmoCube->SetColor(FVector4(0.2f, 0.2f, 0.2f, 1.f));
	GizmoCube->SetRelativeTransform(FTransform(FVector(0, 0, 0), FVector(0, 0, 0), FVector(GizmoSize/2, GizmoSize/2, GizmoSize/2)));
	GizmoCube->SetupAttachment(RootComponent);
	
	XArrowBody = AddComponent<UCylinderComp>();
	XArrowHead = AddComponent<UConeComp>();
	XArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, -90, 0), FVector(1.0f, 1.0f, 1.0f) * GizmoSizeMultiplier));
	XArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.02f, 0.02f, 0.02f) * GizmoSizeMultiplier));
	XArrowBody->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->SetupAttachment(XArrowBody);

	YArrowBody = AddComponent<UCylinderComp>();
	YArrowHead = AddComponent<UConeComp>();
	YArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0, 0), FVector(1.0f, 1.0f, 1.0f) * GizmoSizeMultiplier));
	YArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.02f, 0.02f, 0.02f) * GizmoSizeMultiplier));
	YArrowBody->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->SetupAttachment(YArrowBody);

	ZArrowBody = AddComponent<UCylinderComp>();
	ZArrowHead = AddComponent<UConeComp>();
	ZArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f) * GizmoSizeMultiplier));
	ZArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.02f, 0.02f, 0.02f) * GizmoSizeMultiplier));
	ZArrowBody->SetColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->SetColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->SetupAttachment(ZArrowBody);


	XArrowBody->SetupAttachment(GizmoCube);
	YArrowBody->SetupAttachment(GizmoCube);
	ZArrowBody->SetupAttachment(GizmoCube);

	XRing = AddComponent<URingComp>();
	XRing->SetupAttachment(GizmoCube);
	XRing->SetRelativeTransform(FTransform(FVector(), FVector(0,0,90), FVector(5, 0.1, 5)));
	XRing->SetColor(FVector4(1.0f, 0.f, 0.f, 1.f));
	
	YRing = AddComponent<URingComp>();
	YRing->SetupAttachment(GizmoCube);
	YRing->SetRelativeTransform(FTransform(FVector(), FQuat(), FVector(5 , 0.1, 5)));
	YRing->SetColor(FVector4(0.0f, 1.f, 0.f, 1.f));

	ZRing = AddComponent<URingComp>();
	ZRing->SetupAttachment(GizmoCube);
	ZRing->SetRelativeTransform(FTransform(FVector(), FVector(90,0,0), FVector(5, 0.1, 5)));
	ZRing->SetColor(FVector4(0.f, 0.f, 1.f, 1.f));

	XRing->SetupAttachment(GizmoCube);
	YRing->SetupAttachment(GizmoCube);
	ZRing->SetupAttachment(GizmoCube);

	XRodHead = AddComponent<UCubeComp>();
	XRodHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.01f, 0.01f, 0.01f) * GizmoSizeMultiplier));
	XRodHead->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XRodHead->SetupAttachment(XArrowBody);
	
	YRodHead = AddComponent<UCubeComp>();
	YRodHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.01f, 0.01f, 0.01f) * GizmoSizeMultiplier));
	YRodHead->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YRodHead->SetupAttachment(YArrowBody);

	ZRodHead = AddComponent<UCubeComp>();
	ZRodHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(0.01f, 0.01f, 0.01f) * GizmoSizeMultiplier));
	ZRodHead->SetColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZRodHead->SetupAttachment(ZArrowBody);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowHead);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(XRing);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YRing);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZRing);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(XRodHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YRodHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZRodHead);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(GizmoCube);


	SetActive(false);

	return;
}

void AGizmoHandle::BeginPlay()
{
	AActor::BeginPlay();
}

void AGizmoHandle::Tick(float DeltaTime)
{
	// 모드에 따라서 뜨는 기즈모 종류를 변경
	UpdateGizmoPrimitives();

	AActor* SelectedActor  = FEditorManager::Get().GetSelectedActor();
	if (SelectedActor != nullptr && bIsActive)
	{
		FVector CamPos = FEditorManager::Get().GetCamera()->GetActorRelativeTransform().GetPosition();
		FVector SelectedActorPosition = SelectedActor->GetRootComponent()->GetComponentTransform().GetPosition();
		FTransform GizmoTransform = this->GetRootComponent()->GetRelativeTransform();
		GizmoTransform.SetPosition(SelectedActorPosition);
		
			GizmoTransform.SetRotation(SelectedActor->GetRootComponent()->GetComponentTransform().GetRotation());
		
		/*else
		{
			GizmoTransform.SetRotation(FVector());
		}*/
		//GizmoTransform.SetScale((CamPos - SelectedActorPosition).Length() / 4);
		this->GetRootComponent()->SetRelativeTransform(GizmoTransform);
	}

	AActor::Tick(DeltaTime);

	if (APlayerInput::Get().GetKeyDown(EKeyCode::Space))
	{
 		int type = static_cast<int>(GizmoType);
		type = (type + 1) % static_cast<int>(EGizmoType::Max);
		GizmoType = static_cast<EGizmoType>(type);
	}


	// Gizmo에서 그려주고 있는 bounding box를 그릴지 여부를 결정합니다.
	if (SelectedActor)
	{
		for (auto BoundingBox : SelectedActorBoundingBox)
		{
			BoundingBox->SetIsDefaultRendered(true);
		}
	}
	else
	{
		for (auto BoundingBox : SelectedActorBoundingBox)
		{
			BoundingBox->SetIsDefaultRendered(false);
		}
	}
}

void AGizmoHandle::SetActive(bool bActive)
{
	bIsActive = bActive;
	for (auto& Component : Components)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			PrimitiveComponent->SetIsDefaultRendered(bActive);
		}
	}
	
	// SetActive는 클릭할 때 발생
	// 클릭하면 일단 bounding box를 정리함.
	for (auto BoundingBox : SelectedActorBoundingBox)
	{
		BoundingBox->DetachFromComponent();
		GetWorld()->RemoveRenderComponent(BoundingBox);
		RemoveComponent(BoundingBox);

		GObjects.Remove(BoundingBox->GetUUID());
	}
	SelectedActorBoundingBox.Empty();

	// 만약 active하는 클릭이었다면. bounding box를 생성
	if (bIsActive)
	{
		if (AActor* SelectedActor = FEditorManager::Get().GetSelectedActor())
		{
			for (auto SelectedActorComponent : SelectedActor->GetComponents())
			{
				// primitiveComponent만 처리
				if (auto SelectedActorPrimitive = Cast<UPrimitiveComponent>(SelectedActorComponent))
				{
					// 선택될 수 있는 primitive만 bounding box를 생성(text는 생성 안함)
					if (SelectedActorPrimitive->IsCanPick())
					{
						if (SelectedActorPrimitive->IsA(UBillBoardComponent::StaticClass())) continue;
						auto BoundingBox = AddComponent<UBoundingBoxComponent>();
						SelectedActorBoundingBox.Add(BoundingBox);
						BoundingBox->SetupAttachment(this->RootComponent);
						BoundingBox->SetTargetPrimitive(SelectedActorPrimitive);

						// 원래 Actor에서 해주는데, 동적으로 생성되니 수동으로 호출
						BoundingBox->BeginPlay();
						BoundingBox->RegisterComponentWithWorld(GetWorld());
					}
				}
			}
		}
	}
}

//const char* AGizmoHandle::GetTypeName()
//{
//	return "GizmoHandle";
//}

void AGizmoHandle::DoTransform(FTransform& AT, FVector Result, AActor* Actor )
{
	const FVector& AP = AT.GetPosition();
	FVector WorldDelta = Result - AP;  // 이동할 월드 기준 벡터

	FVector LocalDelta;
	LocalDelta.X = FVector::DotProduct(WorldDelta, AT.GetForward());  // 로컬 X축 이동량
	LocalDelta.Y = FVector::DotProduct(WorldDelta, AT.GetWorldRight());    // 로컬 Y축 이동량
	LocalDelta.Z = FVector::DotProduct(WorldDelta, AT.GetWorldUp());       // 로컬 Z축 이동량


	float MoveSpeedFactor = 0.1f;  // 이동 속도 조절


	if (SelectedAxis == ESelectedAxis::X)
	{
		switch (GizmoType)
		{
		case EGizmoType::Translate:
			AT.MoveLocal(FVector(LocalDelta.X * MoveSpeedFactor, 0, 0));
			break;
		case EGizmoType::Rotate:
			//AT.RotatePitch(Result.X);
			AT.RotateRoll(Result.X);

			break;
		case EGizmoType::Scale:
			AT.AddScale({ Result.X * .1f, 0, 0});
			break;
		}
	}
	else if (SelectedAxis == ESelectedAxis::Y)
	{
		switch (GizmoType)
		{
		case EGizmoType::Translate:
			AT.MoveLocal(FVector(0, LocalDelta.Y * MoveSpeedFactor, 0));
			break;
		case EGizmoType::Rotate:
			AT.RotatePitch(-Result.Y);
			break;
		case EGizmoType::Scale:
			AT.AddScale({ 0, Result.Y * .1f, 0 });
			break;
		}
	}
	else if (SelectedAxis == ESelectedAxis::Z)
	{
		switch (GizmoType)
		{
		case EGizmoType::Translate:
			AT.MoveLocal(FVector(0, 0, LocalDelta.Z * MoveSpeedFactor));
			break;
		case EGizmoType::Rotate:
			AT.RotateYaw(-Result.Z);
			break;
		case EGizmoType::Scale:
			AT.AddScale({0, 0, Result.Z * .1f });
			break;
		}
	}
	Actor->SetActorRelativeTransform(AT);
}

void AGizmoHandle::UpdateGizmoPrimitives()
{
	if (!bIsActive) return;
	bool isTranslate = (GizmoType == EGizmoType::Translate);
	bool isRotate = (GizmoType == EGizmoType::Rotate);
	bool isScale = (GizmoType == EGizmoType::Scale);

	GizmoCube->SetIsDefaultRendered(true);

	// Translation
	XArrowBody->SetIsDefaultRendered(isTranslate || isScale);
	XArrowHead->SetIsDefaultRendered(isTranslate);
	YArrowBody->SetIsDefaultRendered(isTranslate || isScale);
	YArrowHead->SetIsDefaultRendered(isTranslate);
	ZArrowBody->SetIsDefaultRendered(isTranslate || isScale);
	ZArrowHead->SetIsDefaultRendered(isTranslate);

	XArrowBody->SetCanPick(isTranslate || isScale);
	XArrowHead->SetCanPick(isTranslate);
	YArrowBody->SetCanPick(isTranslate || isScale);
	YArrowHead->SetCanPick(isTranslate);
	ZArrowBody->SetCanPick(isTranslate || isScale);
	ZArrowHead->SetCanPick(isTranslate);

	// Rotation
	XRing->SetIsDefaultRendered(isRotate);
	YRing->SetIsDefaultRendered(isRotate);
	ZRing->SetIsDefaultRendered(isRotate);

	XRing->SetCanPick(isRotate);
	YRing->SetCanPick(isRotate);
	ZRing->SetCanPick(isRotate);

	XRodHead->SetIsDefaultRendered(isScale);
	XRodHead->SetCanPick(isScale);
	YRodHead->SetIsDefaultRendered(isScale);
	YRodHead->SetCanPick(isScale);
	ZRodHead->SetIsDefaultRendered(isScale);
	ZRodHead->SetCanPick(isScale);


	GizmoCube->SetCanPick(isTranslate || isScale);
	GizmoCube->SetIsDefaultRendered(isTranslate || isScale);

}

