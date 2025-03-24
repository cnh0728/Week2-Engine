#pragma once
class FRect;

class ISlateViewport
{
public:
	virtual void ChangeMainCamera() = 0;
	virtual void Resize(FRect rect) = 0;
	virtual void SetActiveFullViewport() = 0;
};

