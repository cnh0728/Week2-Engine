#include "SSplitter.h"
#include "Source\Debug\DebugConsole.h"

bool SSplitter::isHover(FVector2 coord) const
{
	return false;
}

void SSplitter::OnFocus()
{
    SetCursor(LoadCursor(NULL, IDC_SIZEALL));
}


bool SSplitter::isClicked(FVector2 mousePos) 
{
    return false;
}

bool SSplitter::OnMouseDrag(FVector2 mouseDelta) 
{
    return false;
}

bool SSplitter::OnMouseUp() 
{
    return false;
}

void SSplitter::ScreenResize(float resizeWidthRatio, float resizeHeightRatio)
{
}


bool SSplitter2x2::isHover(FVector2 coord) const
{
    bool inHorizontal = horitionalHandle.Contains(coord);

    bool inVertical = verticalHandle.Contains(coord);

    /*
    UE_LOG("horitionalHandle (%f, %f) (%f, %f)", 
        horitionalHandle.Min.X, horitionalHandle.Min.Y,
        horitionalHandle.Max.X, horitionalHandle.Max.Y);

    UE_LOG("VerticalHandle (%f, %f) (%f, %f)",
        verticalHandle.Min.X, verticalHandle.Min.Y,
        verticalHandle.Max.X, verticalHandle.Max.Y);
    */
    return inHorizontal || inVertical;
}

void SSplitter2x2::OnFocus()
{
    SetCursor(LoadCursor(NULL, IDC_SIZEALL));
}


bool SSplitter2x2::isClicked(FVector2 mousePos) 
{
    if (horitionalHandle.Contains(mousePos))
    {
        bisHoriClicked = true;
    }
    else if (verticalHandle.Contains(mousePos))
    {
        bisVertiClicked = true;
    }
    return bisHoriClicked || bisVertiClicked;
}

bool SSplitter2x2::OnMouseDrag(FVector2 mouseDelta) 
{
    if (bisHoriClicked)
    {
        horitionalHandle.Max.X += mouseDelta.X;
        horitionalHandle.Min.X += mouseDelta.X;

        SideLT->Rect.Max.X += mouseDelta.X;
        SideLT->Resize(SideLT->Rect);

        SideLB->Rect.Max.X += mouseDelta.X;
        SideLB->Resize(SideLB->Rect);

        SideRT->Rect.Min.X += mouseDelta.X;
        SideRT->Resize(SideRT->Rect);

        SideRB->Rect.Min.X += mouseDelta.X;
        SideRB->Resize(SideRB->Rect);

        //UE_LOG("Hori Splitter Clicked");
    }
    else if (bisVertiClicked)
    {
        verticalHandle.Max.Y += mouseDelta.Y;
        verticalHandle.Min.Y += mouseDelta.Y;

        SideLT->Rect.Max.Y += mouseDelta.Y;
        SideLT->Resize(SideLT->Rect);

        SideRT->Rect.Max.Y += mouseDelta.Y;
        SideRT->Resize(SideRT->Rect);

        SideLB->Rect.Min.Y += mouseDelta.Y;
        SideLB->Resize(SideLB->Rect);

        SideRB->Rect.Min.Y += mouseDelta.Y;
        SideRB->Resize(SideRB->Rect);

        //UE_LOG("Verti Splitter Clicked");
    }
    return false;
}

bool SSplitter2x2::OnMouseUp() 
{
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    bisHoriClicked = false;
    bisVertiClicked = false;
    return false;
}

void SSplitter2x2::ScreenResize(float resizeWidthRatio, float resizeHeightRatio)
{
    horitionalHandle.ResizeRatio(resizeWidthRatio, resizeHeightRatio);
    verticalHandle.ResizeRatio(resizeWidthRatio, resizeHeightRatio);
}
