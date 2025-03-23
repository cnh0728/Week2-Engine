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
		viewport->ChangeMainCamera();
		return true;
	}
	return false;
}

void SWindow::OnFocus()
{
}


bool SWindow::isClicked(FVector2 mousePos) 
{
	return Rect.Contains(mousePos);
}

bool SWindow::OnMouseDrag(FVector2 mouseDelta)
{
	return false;
}

bool SWindow::OnMouseUp()
{
	return false;
}

void SWindow::Resize(const FRect& _rect)
{
	Rect = _rect;
	viewport->Resize(_rect);
}
