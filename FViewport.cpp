#include "FViewport.h"
#include "Source/Core/Math/FRect.h"
#include "../SWindow.h"
#include <d3d11.h>
FViewport::FViewport()
{
	viewport = new D3D11_VIEWPORT();
}
FViewport::~FViewport()
{
	delete viewport;
}
void FViewport::SetSWindow(SWindow* _refSWindow)
{
	refSWindow = _refSWindow;
}

void FViewport::SetCamera(ACamera* _camera)
{
	camera = _camera;
}

void FViewport::UpdateSWindowSize()
{
	if (refSWindow&&viewport)
	{
		FRect SWindowRect = refSWindow->Rect;
		FVector2 min = SWindowRect.Min;
		FVector2 max = SWindowRect.Max;
		*viewport = { min.X,min.Y,max.X - min.X,max.Y - min.Y,0.0f,1.0f };
	}
}
