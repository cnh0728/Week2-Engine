#pragma once
#include "Core/HAL/PlatformType.h"
#include "ISlateViewport.h"
#include "Core/Math/FRect.h"

class ID3D11RasterizerState;
class SWindow;

namespace ECameraViewMode { enum class Type : uint8; }

class FViewport : public ISlateViewport
{
public:
	FViewport(FRect rect);
	virtual ~FViewport();
	//void SetViewport();
	void SetViewportRendering();
	void SetCamera(ECameraViewMode::Type _cameraType);
	ECameraViewMode::Type GetCameraType();
	virtual void ChangeMainCamera() override;
	virtual void Resize(FRect rect) override;
	virtual void SetActiveFullViewport() override;
	const FRect& GetRect();
private:
	SWindow* refSWindow;
	ECameraViewMode::Type cameraType;
	FRect Rect;



	//ID3D11RasterizerState* rasterizerState;
};

