#include "FSlateApplication.h"
#include "Source/Core/Rendering/URenderer.h"

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
	windowLT->Rect.Min = FVector2(halfWidth, 0);
	windowLT->Rect.Max = FVector2(halfWidth*2, halfHeight);
	windows.Add(windowRT);

	SWindow* windowLB = new SWindow();
	windowLT->Rect.Min = FVector2(0, halfHeight);
	windowLT->Rect.Max = FVector2(halfWidth, halfHeight*2);
	windows.Add(windowLB);

	SWindow* windowRB = new SWindow();
	windowLT->Rect.Min = FVector2(halfWidth, halfHeight);
	windowLT->Rect.Max = FVector2(halfWidth*2, halfHeight*2);
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
	for (int i=0;i<windows.Len();i++)
	{
		SWindow* window = windows[i];
		delete window;
	}
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

	for (int i = 0; i < windows.Len(); i++)
	{
		windows[i]->isHover(mouse);
	}
}
