#include "ConfigManager.h"
#include <fstream>
#include "Data/SettingsLoader.h"

ConfigManager::ConfigManager()
{
    SettingsList.Add(new USettingsLoader(FString("editor.ini")));
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
