#pragma once
#include "Core/HAL/PlatformType.h"

class ID3D11RasterizerState;
class SWindow;

namespace ECameraViewMode { enum class Type : uint8; }

class FViewport
{
public:
	FViewport();
	virtual ~FViewport();
	//void SetViewport();
	void SetSWindow(SWindow* refSWindow);
	void SetCamera(ECameraViewMode::Type _cameraType);
	void SetViewportRendering();
	void UpdateSWindowSize();
private:
	SWindow* refSWindow;
	ECameraViewMode::Type cameraType;

	//ID3D11RasterizerState* rasterizerState;
};

