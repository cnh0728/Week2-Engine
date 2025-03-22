#pragma once
//#include <d3d11.h>
class D3D11_VIEWPORT;
class ACamera;
class ID3D11RasterizerState;
class SWindow;
class FViewport
{
public:
	FViewport();
	virtual ~FViewport();
	//void SetViewport();
	void SetSWindow(SWindow* refSWindow);
	void SetCamera(ACamera* camera);
	void UpdateSWindowSize();
private:
	ACamera* camera;
	D3D11_VIEWPORT* viewport;
	ID3D11RasterizerState* rasterizerState;
	SWindow* refSWindow;
};

