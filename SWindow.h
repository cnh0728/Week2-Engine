#pragma once
#include "Source\Core\Math\FRect.h"

class ISlateViewport;

class SWindow
{
public:
	SWindow();
	SWindow(FRect _rect);
	virtual ~SWindow();
	FRect Rect;
	void SetISlateViewport(ISlateViewport* _viewport);
	void AttachViewportCamera();
	virtual bool isHover(FVector2 coord) const;
	virtual void OnFocus();
	virtual bool isClicked(FVector2 mousePos) ;
	virtual bool OnMouseDrag(FVector2 mouseDelta) ;
	virtual bool OnMouseUp();
	virtual void Resize(const FRect& _rect);
	virtual void ScreenResize(float resizeWidthRatio, float resizeHeightRatio);
	void SetActiveFullViewport();
	void RestorePrevSize();
	void ChangeMainCamera();
private:
	bool bisHover = false;
	bool bisClicked = false;
	ISlateViewport* viewport;
	FRect prevRect;
};

