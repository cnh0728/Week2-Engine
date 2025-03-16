#pragma once
#include <vector>
#include "ImGui/imgui.h"

#define UE_LOG(format, ...) Debug::Log(format, ##__VA_ARGS__)


class FString;

class Debug
{
    static std::vector<FString> items; // 출력 로그

public:
    static void ShowConsole(bool bWasWindowSizeUpdated);
    static void ProcessCommand(const FString& command, std::vector<FString>& log);
    static void Log(const char* format, ...);
};
