#pragma once
#include "Source\Core\Math\FRect.h"

class FRect;
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
private:
	bool bisHover;
	ISlateViewport* viewport;
};

