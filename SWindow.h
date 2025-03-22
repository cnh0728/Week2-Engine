#pragma once
#include "Source\Core\Math\FRect.h"
class SWindow
{
public:
	SWindow();
	SWindow(FRect _rect);
	~SWindow();
	FRect Rect;
	virtual bool isHover(FVector2 coord) const;
private:
	bool bisHover;
};

