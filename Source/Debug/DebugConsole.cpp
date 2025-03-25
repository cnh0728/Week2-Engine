
#include "Debug/DebugConsole.h"
#include <cstdarg>
#include <algorithm>
#include "Core/Container/String.h"
#include "Core/Engine.h"
#include "ImGui/imgui_internal.h"

std::vector<FString> Debug::items;


void Debug::ShowConsole(bool bWasWindowSizeUpdated)
{    
    static char inputBuf[256] = "";
    static std::vector<FString> history;
    static int historyPos = -1;
    bool reclaimFocus = false;

    float windowWidth = UEngine::Get().GetScreenWidth();
    float windowHeight = UEngine::Get().GetScreenHeight();

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    float consoleWindowWidth = static_cast<float>(windowWidth) * 1.f;
    float consoleWindowHeight = static_cast<float>(windowHeight) * 0.2f;
    float consoleWindowPosX = (static_cast<float>(windowWidth) - consoleWindowWidth) * 0.5f;
    float consoleWindowPosY = (static_cast<float>(windowHeight) - consoleWindowHeight) * 1.f;
    //ImGui::SetNextWindowPos(ImVec2(consoleWindowPosX, consoleWindowPosY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(consoleWindowWidth, consoleWindowHeight), ImGuiCond_Always);

    ImGui::Begin("Console");

     if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar))
     {
         for (const auto& Item : items)
             ImGui::TextUnformatted(*Item);
    
         if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
             ImGui::SetScrollHereY(1.0f);
     }
     ImGui::EndChild();

     if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf),
         ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory,
         [](ImGuiInputTextCallbackData* data) -> int
         {
             if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
             {
                 if (history.empty()) return 0;
                 historyPos += (data->EventKey == ImGuiKey_UpArrow) ? -1 : 1;
                 historyPos = std::clamp(historyPos, 0, (int)history.size() - 1);
    
                 FString& historyCommand = history[historyPos];
                 data->DeleteChars(0, data->BufTextLen);
                 data->InsertChars(0, *historyCommand);
             }
             return 0;
         }))
     {
         FString inputStr = inputBuf;
         if (!inputStr.IsEmpty())
         {
             items.push_back("> " + inputStr);
             history.push_back(inputStr);
             historyPos = (int)history.size();
             ProcessCommand(inputStr, items);
         }
         inputBuf[0] = '\0';
         reclaimFocus = true;
     }

     if (reclaimFocus)
         ImGui::SetKeyboardFocusHere(-1);
    ImGui::End();
}

void Debug::ProcessCommand(const FString& command, std::vector<FString>& log)
{
    log.push_back("Executing: " + command);

    if (command == "clear")
    {
        log.clear();
    }
    else if (command == "help")
    {
        log.push_back("Available commands:");
        log.push_back("- clear: Clears the console.");
        log.push_back("- help: Shows this help message.");
    }
    else
    {
        log.push_back("Unknown command: " + command);
    }
}

void Debug::Log(const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    items.emplace_back(buffer);
}
