#include "UObject.h"
#include "Object/Class/Class.h"

UObject::UObject()
{

}

UObject::~UObject()
{
}

// UObject를 담는 UClass는 nullptr을 parent로 가짐
UClass* UObject::StaticClass()
{
	static UClass ClassPrivate(nullptr, "UObject");
	return &ClassPrivate;
}



