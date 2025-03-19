#include "GizmoHandle.h"

#include "Object/Actor/Camera.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"
#include <Core/Input/PlayerInput.h>

AGizmoHandle::AGizmoHandle()
{
	bIsGizmo = true;
	const float GizmoSize = 0.2f;
	const float GizmoSizeMultiplier = 1 / GizmoSize;
	// gizmo 중심을 잡기 위한 가상의 component
	USceneComponent* GizmoCenter = AddComponent<USceneComponent>();
	SetRootComponent(GizmoCenter);

	UCubeComp* GizmoCube = AddComponent<UCubeComp>();
	GizmoCube->SetColor(FVector4(0.2f, 0.2f, 0.2f, 1.f));
	GizmoCube->SetRelativeTransform(FTransform(FVector(0, 0, 0), FVector(0, 0, 0), FVector(GizmoSize, GizmoSize, GizmoSize)));
	GizmoCube->SetupAttachment(RootComponent);
	
	UCylinderComp* XArrowBody = AddComponent<UCylinderComp>();
	UConeComp* XArrowHead = AddComponent<UConeComp>();
	XArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, -90, 0), FVector(1.0f, 1.0f, 2.0f) * GizmoSizeMultiplier));
	XArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.45f), FQuat(0, 0, 0, 1), FVector(0.05f, 0.05f, 0.02f) * GizmoSizeMultiplier));
	XArrowBody->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->SetupAttachment(XArrowBody);

	UCylinderComp* YArrowBody = AddComponent<UCylinderComp>();
	UConeComp* YArrowHead = AddComponent<UConeComp>();
	YArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0, 0), FVector(1.0f, 1.0f, 2.0f) * GizmoSizeMultiplier));
	YArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.45f), FQuat(0, 0, 0, 1), FVector(0.05f, 0.05f, 0.02f) * GizmoSizeMultiplier));
	YArrowBody->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->SetupAttachment(YArrowBody);

	UCylinderComp* ZArrowBody = AddComponent<UCylinderComp>();
	UConeComp* ZArrowHead = AddComponent<UConeComp>();
	ZArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 0, 0), FVector(1.0f, 1.0f, 2.0f) * GizmoSizeMultiplier));
	ZArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.45f), FQuat(0, 0, 0, 1), FVector(0.05f, 0.05f, 0.02f) * GizmoSizeMultiplier));
	ZArrowBody->SetColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->SetColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->SetupAttachment(ZArrowBody);


	XArrowBody->SetupAttachment(GizmoCube);
	YArrowBody->SetupAttachment(GizmoCube);
	ZArrowBody->SetupAttachment(GizmoCube);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowHead);
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
	AActor* SelectedActor  = FEditorManager::Get().GetSelectedActor();
	if (SelectedActor != nullptr && bIsActive)
	{
		FVector SelectedActorPosition = SelectedActor->GetRootComponent()->GetRelativeTransform().GetPosition();
		FTransform GizmoTransform = this->GetRootComponent()->GetRelativeTransform();
		GizmoTransform.SetPosition(SelectedActorPosition);
		this->GetRootComponent()->SetRelativeTransform(GizmoTransform);
	}

	SetScaleByDistance();
	
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
			BoundingBox->SetCanBeRendered(true);
		}
	}
	else
	{
		for (auto BoundingBox : SelectedActorBoundingBox)
		{
			BoundingBox->SetCanBeRendered(false);
		}
	}
}

void AGizmoHandle::SetScaleByDistance()
{
	FTransform MyTransform = GetActorRelativeTransform();
	
	// 액터의 월드 위치
	FVector actorWorldPos = MyTransform.GetPosition();

	FTransform CameraTransform = FEditorManager::Get().GetCamera()->GetActorRelativeTransform();
	
	// 카메라의 월드 위치
	FVector cameraWorldPos = CameraTransform.GetPosition();

	// 거리 계산
	float distance = (actorWorldPos - cameraWorldPos).Length();

	float baseScale = 1.0f;    // 기본 스케일
	float scaleFactor = distance * 0.1f; // 거리에 비례하여 스케일 증가

	// float minScale = 1.0f;     // 최소 스케일
	// float maxScale = 1.0f;     // 최대 스케일
	// float scaleFactor = clamp(1.0f / distance, minScale, maxScale);

	MyTransform.SetScale(scaleFactor, scaleFactor, scaleFactor);
}

void AGizmoHandle::SetActive(bool bActive)
{
	bIsActive = bActive;
	for (auto& Component : Components)
	{
		if (UPrimitiveComponent* PrimitiveComponent = dynamic_cast<UPrimitiveComponent*>(Component))
		{
			PrimitiveComponent->SetCanBeRendered(bActive);
		}
	}
	
	// SetActive는 클릭할 때 발생
	// 클릭하면 일단 bounding box를 정리함.
	for (auto BoundingBox : SelectedActorBoundingBox)
	{
		BoundingBox->DetachFromComponent();
		GetWorld()->RemoveRenderComponent(BoundingBox);
		RemoveComponent(BoundingBox);

		UEngine::Get().GObjects.Remove(BoundingBox->GetUUID());
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
				if (auto SelectedActorPrimitive = dynamic_cast<UPrimitiveComponent*>(SelectedActorComponent))
				{
					// 선택될 수 있는 primitive만 bounding box를 생성(text는 생성 안함)
					if (SelectedActorPrimitive->IsCanPick())
					{
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

	if (SelectedAxis == ESelectedAxis::X)
	{
		switch (GizmoType)
		{
		case EGizmoType::Translate:
			AT.SetPosition({ Result.X, AP.Y, AP.Z });
			break;
		case EGizmoType::Rotate:
			AT.RotatePitch(Result.X);
			break;
		case EGizmoType::Scale:
			AT.AddScale({ Result.X * .1f, 0, AP.Z * .1f });
			break;
		}
	}
	else if (SelectedAxis == ESelectedAxis::Y)
	{
		switch (GizmoType)
		{
		case EGizmoType::Translate:
			AT.SetPosition({ AP.X, Result.Y, AP.Z });
			break;
		case EGizmoType::Rotate:
			AT.RotateRoll(Result.Y);
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
			AT.SetPosition({ AP.X, AP.Y, Result.Z });
			break;
		case EGizmoType::Rotate:
			AT.RotatePitch(-Result.Z);
			break;
		case EGizmoType::Scale:
			AT.AddScale({0, 0, Result.Z * .1f });
			break;
		}
	}
	Actor->SetActorRelatvieTransform(AT);
}

