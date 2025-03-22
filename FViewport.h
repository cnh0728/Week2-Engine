#pragma once
class D3D11_VIEWPORT;
class ACamera;
class ID3D11RasterizerState;
class FViewport
{
public:
	FViewport();
	//void SetViewport();
private:
	ACamera* camera;
	D3D11_VIEWPORT* viewport;
	ID3D11RasterizerState* rasterizerState;
};

