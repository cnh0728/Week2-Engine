#include "FSlateApplication.h"
#include "Source/Core/Rendering/URenderer.h"
#include "Source/Static/FEditorManager.h"
void FSlateApplication::Initialize()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	FVector2 windowSize = Renderer->GetSwapChainSize();

	float halfWidth = windowSize.X / 2.0f;
	float halfHeight = windowSize.Y / 2.0f;

	SWindow* windowLT = new SWindow();
	windowLT->Rect.Min = FVector2(0, 0);
	windowLT->Rect.Max = FVector2(halfWidth, halfHeight);
	windows.Add(windowLT);

	SWindow* windowRT = new SWindow();
	windowRT->Rect.Min = FVector2(halfWidth, 0);
	windowRT->Rect.Max = FVector2(halfWidth*2, halfHeight);
	windows.Add(windowRT);

	SWindow* windowLB = new SWindow();
	windowLB->Rect.Min = FVector2(0, halfHeight);
	windowLB->Rect.Max = FVector2(halfWidth, halfHeight*2);
	windows.Add(windowLB);

	SWindow* windowRB = new SWindow();
	windowRB->Rect.Min = FVector2(halfWidth, halfHeight);
	windowRB->Rect.Max = FVector2(halfWidth*2, halfHeight*2);
	windows.Add(windowRB);
	
	SSplitter2x2* sspliter2by2 = new SSplitter2x2();
	sspliter2by2->SideLT = windowLT;
	sspliter2by2->SideRT = windowRT;
	sspliter2by2->SideLB = windowLB;
	sspliter2by2->SideRB = windowRB;

	windows.Add(sspliter2by2);
}

void FSlateApplication::Tick()
{
	ProcessIsHover();
	ProcessKeyDownEvent();
	ProcessMouseButtonDownEvent();
}

void FSlateApplication::ShutDown()
{
	for (int i=0;i<windows.Num();i++)
	{
		SWindow* window = windows[i];
		delete window;
	}
}

FRect FSlateApplication::GetCurrentWindow()
{
	return currentWindow->Rect;
}

void FSlateApplication::ProcessMouseButtonDownEvent()
{
}

void FSlateApplication::ProcessKeyDownEvent()
{

}

void FSlateApplication::ProcessIsHover()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
	FVector2 mouse(pt.x, pt.y);
	for (int i = 0; i < windows.Num(); i++)
	{
		if (windows[i]->isHover(mouse))
		{
			currentWindow = windows[i];
			FEditorManager::Get().SetCameraIndex(i);
		}
	}
}
