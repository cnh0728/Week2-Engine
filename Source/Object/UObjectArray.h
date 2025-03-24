#pragma once
#include "Core/Container/Map.h"
#include "Object/UObject.h"

extern TMap<uint32, std::shared_ptr<UObject>> GObjects;
