#pragma once
#include "SWindow.h"

class SSplitter :
    public SWindow
{
public:
    virtual bool isHover(FVector2 coord) const override;
    virtual void OnFocus() override;
    virtual bool isClicked(FVector2 mousePos)  override;
    virtual bool OnMouseDrag(FVector2 mouseDelta) ;
    virtual bool OnMouseUp()  override;
    virtual void ScreenResize(float resizeWidthRatio, float resizeHeightRatio) override;
    SWindow* SideLT;
    SWindow* SideRB;
    FRect Handle;
};


class SSplitter2x2 : public SWindow
{
public:
    virtual bool isHover(FVector2 coord) const override;
    virtual void OnFocus() override;
    virtual bool isClicked(FVector2 mousePos)  override;
    virtual bool OnMouseDrag(FVector2 mouseDelta) ;
    virtual bool OnMouseUp()  override;
    virtual void ScreenResize(float resizeWidthRatio, float resizeHeightRatio) override;
    SWindow* SideLT;
    SWindow* SideLB;
    SWindow* SideRB;
    SWindow* SideRT;
    FRect horitionalHandle;
    FRect verticalHandle;
private:
    bool bisHoriClicked = false;
    bool bisVertiClicked = false;
};