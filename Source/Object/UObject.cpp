#include "UObject.h"

uint32_t UObject::InstanceCounter = 0;

UObject::UObject()
{
	Name = FName((std::string("UObject") + std::to_string(InstanceCounter++)).c_str());
}

UObject::~UObject()
{
}
