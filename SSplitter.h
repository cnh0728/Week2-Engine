#pragma once
#include "SWindow.h"

class SSplitter :
    public SWindow
{
public:
    virtual bool isHover(FVector2 coord) const override;
    SWindow* SideLT;
    SWindow* SideRB;
};


class SSplitter2x2 : public SWindow
{
public:
    virtual bool isHover(FVector2 coord) const override;
    SWindow* SideLT;
    SWindow* SideLB;
    SWindow* SideRB;
    SWindow* SideRT;
private:
    FRect verticalHandle;
    FRect horitionalHandle;
};