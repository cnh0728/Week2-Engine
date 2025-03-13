#include "GizmoHandle.h"

#include "Object/Actor/Camera.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"
#include <Core/Input/PlayerInput.h>

AGizmoHandle::AGizmoHandle()
{
	bIsGizmo = true;
	const float GizmoSize = 0.05;
	const float GizmoSizeMultiplier = 1 / GizmoSize;

	UCubeComp* GizmoCenter = AddComponent<UCubeComp>();
	SetRootComponent(GizmoCenter);
	GizmoCenter->SetCustomColor(FVector4(0.2f, 0.2f, 0.2f, 1.f));
	GizmoCenter->SetRelativeTransform(FTransform(FVector(0, 0, 0), FVector(0, 0, 0), FVector(GizmoSize, GizmoSize, GizmoSize)));

	UCylinderComp* XArrowBody = AddComponent<UCylinderComp>();
	UConeComp* XArrowHead = AddComponent<UConeComp>();
	XArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, -90, 0), FVector(0.02f, 0.02f, 0.833333f) * GizmoSizeMultiplier));
	XArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 1.0f), FQuat(0, 0, 0, 1), FVector(3.f, 3.f, 0.2f)));
	XArrowBody->SetCustomColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->SetCustomColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	XArrowHead->AttachToComponent(XArrowBody);

	UCylinderComp* YArrowBody = AddComponent<UCylinderComp>();
	UConeComp* YArrowHead = AddComponent<UConeComp>();
	YArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0,0), FVector(0.02f, 0.02f, 0.833333f) * GizmoSizeMultiplier));
	YArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 1.0f), FQuat(0, 0, 0, 1), FVector(3.f, 3.f, 0.2f)));
	YArrowBody->SetCustomColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->SetCustomColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	YArrowHead->AttachToComponent(YArrowBody);

	UCylinderComp* ZArrowBody = AddComponent<UCylinderComp>();
	UConeComp* ZArrowHead = AddComponent<UConeComp>();
	ZArrowBody->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 0, 0), FVector(0.02f, 0.02f, 0.833333f) * GizmoSizeMultiplier));
	ZArrowHead->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 1.0f), FQuat(0, 0, 0, 1), FVector(3.f, 3.f, 0.2f)));
	ZArrowBody->SetCustomColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->SetCustomColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	ZArrowHead->AttachToComponent(ZArrowBody);


	XArrowBody->AttachToComponent(GizmoCenter);
	YArrowBody->AttachToComponent(GizmoCenter);
	ZArrowBody->AttachToComponent(GizmoCenter);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowBody);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrowHead);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(GizmoCenter);



	SetActive(false);

	//// !NOTE : Z방향으로 서있음
	//// z
	//UCylinderComp* ZArrow = AddComponent<UCylinderComp>();
	//ZArrow->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1, 1, 1)));
	//ZArrow->SetCustomColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));
	//CylinderComponents.Add(ZArrow);

	//// x
	//UCylinderComp* XArrow = AddComponent<UCylinderComp>();
	//XArrow->AttachToComponent(ZArrow);
	//XArrow->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 90.0f, 0.0f), FVector(1, 1, 1)));
	//XArrow->SetCustomColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	//CylinderComponents.Add(XArrow);


	//// y
	//UCylinderComp* YArrow = AddComponent<UCylinderComp>();
	//YArrow->AttachToComponent(ZArrow);
	//YArrow->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90.0f, 0.0f, 0.0f), FVector(1, 1, 1)));
	//YArrow->SetCustomColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	//CylinderComponents.Add(YArrow);
	//RootComponent = ZArrow;
	
	//UEngine::Get().GetWorld()->AddZIgnoreComponent(ZArrow);
	//UEngine::Get().GetWorld()->AddZIgnoreComponent(XArrow);
	//UEngine::Get().GetWorld()->AddZIgnoreComponent(YArrow);

	SetActive(false);
}

void AGizmoHandle::Tick(float DeltaTime)
{
	
	AActor* SelectedActor  = FEditorManager::Get().GetSelectedActor();
	if (!SelectedActor || !bIsActive)
	{
		AActor::Tick(DeltaTime); // Gizmo update
		return;
	}

	// 기즈모가 붙어있는 상태
	FTransform ActorTransform = SelectedActor->GetActorTransform();
	FTransform GizmoTransform;
	GizmoTransform.SetPosition(ActorTransform.GetPosition());
	if (bIsWorldFixed)
	{
		GizmoTransform.SetRotation(ActorTransform.GetRotation());
	}
	//this->SetActorTransform(GizmoTransform);
	//return;





	
	if (SelectedActor != nullptr && bIsActive)
	{
		// Actor의 정보 받아오기
		FTransform ActorTr = SelectedActor->GetActorTransform();
		FTransform GizmoTr;
		GizmoTr.SetPosition(ActorTr.GetPosition());
		GizmoTr.SetRotation(ActorTr.GetRotation());
		GizmoTr.SetScale(this->GetRootComponent()->GetRelativeTransform().GetScale());
		SetActorTransform(GizmoTr);
		// Gizmo의 위치정보를 받아옵니다.
		//FTransform GizmoTr = SelectedActor->GetRootComponent()->GetRelativeTransform();
		// Gizmo 크기는 1로 고정
		//GizmoTr.SetScale(FVector(1, 1, 1));
		//GizmoTr.SetPosition(SelectedActor->GetActorTransform().GetPosition());
		// World에 고정되지 않았으면 Rotation도 가져옵니다.
		//if (bIsWorldFixed || true)
		//{
			//GizmoTr.SetRotation(RootComponent->GetRelativeTransform().GetRotation());
		//}
		// Actor의 Root component == 위치정보를 수정합니다.
		//SetActorTransform(GizmoTr);
	}

	SetScaleByDistance();
	

	if (SelectedAxis != ESelectedAxis::None)
	{
		if (AActor* Actor = FEditorManager::Get().GetSelectedActor())
		{
			// 마우스의 커서 위치를 가져오기
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
			
			RECT Rect;
			GetClientRect(UEngine::Get().GetWindowHandle(), &Rect);
			int ScreenWidth = Rect.right - Rect.left;
			int ScreenHeight = Rect.bottom - Rect.top;
			
			// 커서 위치를 NDC로 변경
			float PosX = 2.0f * pt.x / ScreenWidth - 1.0f;
			float PosY = -2.0f * pt.y / ScreenHeight + 1.0f;
			
			// Projection 공간으로 변환
			FVector4 RayOrigin {PosX, PosY, 0.0f, 1.0f};
			FVector4 RayEnd {PosX, PosY, 1.0f, 1.0f};
			
			// View 공간으로 변환
			FMatrix InvProjMat = UEngine::Get().GetRenderer()->GetProjectionMatrix().Inverse();
			RayOrigin = InvProjMat.TransformVector4(RayOrigin);
			RayEnd = InvProjMat.TransformVector4(RayEnd);
			//RayOrigin.W = 1;
			//RayEnd *= 1000.0f;  // 프러스텀의 Far 값이 적용이 안돼서 수동으로 곱함
			//RayEnd.W = 1;
			
			// 마우스 포인터의 월드 위치와 방향
			FMatrix InvViewMat = FEditorManager::Get().GetCamera()->GetViewMatrix().Inverse();
			RayOrigin = InvViewMat.TransformVector4(RayOrigin);
			RayEnd = InvViewMat.TransformVector4(RayEnd);
			//RayOrigin /= RayOrigin.W = 1;
			//RayEnd /= RayEnd.W = 1;
			
			FVector RayOriginWorld = FVector(RayOrigin.X / RayOrigin.W, RayOrigin.Y / RayOrigin.W, RayOrigin.Z / RayOrigin.W);
			FVector RayEndWorld = FVector(RayEnd.X / RayEnd.W, RayEnd.Y / RayEnd.W, RayEnd.Z / RayEnd.W);

			FVector RayDir = (RayEndWorld - RayOriginWorld).GetSafeNormal();
	
			// 액터와의 거리
			float Distance = FVector::Distance(RayOriginWorld, Actor->GetActorTransform().GetPosition());
			
			// Ray 방향으로 Distance만큼 재계산
			FVector Result = RayOriginWorld + RayDir * Distance;
	
			FTransform AT = Actor->GetActorTransform();

			DoTransform(AT, Result, Actor);
			
		}
	}

	if (APlayerInput::Get().GetKeyDown(EKeyCode::Space))
	{
 		int type = static_cast<int>(GizmoType);
		type = (type + 1) % static_cast<int>(EGizmoType::Max);
		GizmoType = static_cast<EGizmoType>(type);
	}

}

void AGizmoHandle::SetScaleByDistance()
{
	FTransform MyTransform = GetActorTransform();
	
	// 액터의 월드 위치
	FVector actorWorldPos = MyTransform.GetPosition();

	FTransform CameraTransform = FEditorManager::Get().GetCamera()->GetActorTransform();
	
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
	for (auto& Cylinder : CylinderComponents)
	{
		Cylinder->SetCanBeRendered(bActive);
	}
}

const char* AGizmoHandle::GetTypeName()
{
	return "GizmoHandle";
}

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
	Actor->SetActorTransform(AT);
}

