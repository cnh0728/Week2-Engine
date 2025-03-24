#pragma once
#include <string>
#include <vector>

#include "Core/Container/Array.h"

class ObjectLoader {
public:
    ObjectLoader() = default;
    ~ObjectLoader() = default;
    bool LoadFromFile(const std::string& Filename);
    size_t Hash(std::string Str);
    TArray<std::string> Split(const std::string& str, char delim);
};
