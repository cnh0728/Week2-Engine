#include "ConfigManager.h"
#include <fstream>
#include "Data/UCameraSettings.h"

ConfigManager::ConfigManager()
{
    SettingsList.Add(new UCameraSettings(FString("CameraConfig.ini")));
}

ConfigManager::~ConfigManager()
{
    for (UDeveloperSettings* Setting : SettingsList)
    {
        delete Setting;
    }
}

void ConfigManager::LoadAllConfigs()
{
    for (UDeveloperSettings* Setting : SettingsList)
    {
        if (Setting)
        {
            Setting->LoadConfig();
        }
    }
}

void ConfigManager::SaveAllConfigs()
{
    for (UDeveloperSettings* Setting : SettingsList)
    {
        if (Setting)
        {
            Setting->SaveConfig();
        }
    }
}
