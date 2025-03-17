#pragma once
#define _TCHAR_DEFINED
#include <Windows.h>
#include "ImGui/imgui.h"
#include "Core/Container/Set.h"
#include "Core/Container/Name.h"

class FName;
class AActor;
class URenderer;

class UI
{
public:
    int currentItem = 0;
    int NumOfSpawn = 0;
	bool bIsInitialized = false;
    
    void Initialize(HWND hWnd, URenderer& Renderer, UINT ScreenWidth, UINT ScreenHeight);
    void Update();
    void Shutdown();

	void OnUpdateWindowSize(UINT InScreenWidth, UINT InScreenHeight);

public:// UIWindows
    void RenderControlPanel();
    void RenderWorldGridSetting();
    void RenderMemoryUsage();
    void RenderPrimitiveSelection();
    void RenderCameraSettings();
	void RenderViewMode();
	void RenderShowFlag();
    void RenderPropertyWindow();
	void RenderSceneManager();

private:
	bool bWasWindowSizeUpdated = true;
    ImVec2 ScreenSize;
	ImVec2 InitialScreenSize;
	URenderer* Renderer;
    float windowWidth;
	float windowHeight;
	
    
    void SetWindowLayout(float widthRatio, float heightRatio, float posXRatio, float posYRatio);
    TArray<FName> Unselectables;
};
