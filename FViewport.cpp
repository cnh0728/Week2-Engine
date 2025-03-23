#include "FViewport.h"
#include "Source/Core/Math/FRect.h"
#include "../SWindow.h"
#include "Static/FEditorManager.h"
#include "Core/Engine.h"
#include "Object/Actor/Camera.h"
FViewport::FViewport()
{
}
FViewport::~FViewport()
{
}
void FViewport::SetSWindow(SWindow* _refSWindow)
{
	refSWindow = _refSWindow;
}

void FViewport::SetCamera(ECameraViewMode::Type _cameraType)
{
	cameraType = _cameraType;
}

void FViewport::SetViewportRendering()
{
	URenderer* renderer = UEngine::Get().GetRenderer();
	D3D11_VIEWPORT viewport;
	if (refSWindow)
	{
		FRect SWindowRect = refSWindow->Rect;
		FVector2 min = SWindowRect.Min;
		FVector2 max = SWindowRect.Max;
		viewport = { min.X,min.Y,max.X - min.X,max.Y - min.Y,0.0f,1.0f };
		renderer->SetViewportRendering(viewport);
	}

	FEditorManager::Get().SetOrthoCamera(cameraType);
	ACamera* cam = FEditorManager::Get().GetCamera();
	renderer->UpdateViewMatrix(cam->GetActorRelativeTransform());
	renderer->UpdateProjectionMatrix(cam);

}

void FViewport::UpdateSWindowSize()
{
	

	
}
