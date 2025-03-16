#pragma once

#include <vector>
#include "Data/UDeveloperSettings.h"
#include "SettingsLoader.h" 
#include "Core/AbstractClass/Singleton.h"
#include "Core/Container/Array.h"

class ConfigManager : public TSingleton<ConfigManager>
{
public:
    ConfigManager();
    ~ConfigManager();
    void LoadAllConfigs();
    void SaveAllConfigs();

private:
    ConfigManager& operator=(const ConfigManager&) = delete;
    TArray<UDeveloperSettings*> SettingsList;
};