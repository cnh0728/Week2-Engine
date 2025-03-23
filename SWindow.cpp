#include "SWindow.h"
#include "Source\Debug\DebugConsole.h"
#include "ISlateViewport.h"
SWindow::SWindow()
{
}

SWindow::SWindow(FRect _rect)
{
	Rect = _rect;
}

SWindow::~SWindow()
{
}

void SWindow::SetISlateViewport(ISlateViewport* _viewport)
{
	viewport = _viewport;
}

void SWindow::AttachViewportCamera()
{
	viewport->ChangeMainCamera();
}

bool SWindow::isHover(FVector2 coord) const
{
	float x = coord.X;
	float y = coord.Y;
	if (Rect.Min.X <= x && x <= Rect.Max.X && Rect.Min.Y <= y && y <= Rect.Max.Y)
	{
		UE_LOG("Min: (%f,%f), Max: (%f,%f) IS HOVER",
			Rect.Min.X, Rect.Min.Y, Rect.Max.X, Rect.Max.Y);
		//bisHover = true;
		return true;
	}
	//bisHover = false;
	return false;
}
