#pragma once

#define INVALID_ID -1
#define INVALID_INDEX -1
#define INVALID_DATA_KEY TEXT("")

#define PLAYER_INDEX 0

#define MAX_UNIT_COUNT 50000
#define UNIT_SCALE_1 1
#define UNIT_SCALE_2 2
#define UNIT_SCALE_3 3
#define UNIT_SCALE_4 4
#define UNIT_SCALE_5 5

#define OBJECT_TYPE_NOT_ACTIVE_BUILDING_UNIT ECC_GameTraceChannel4
#define OBJECT_TYPE_BUILDING_UNIT ECC_GameTraceChannel5
#define TRACE_TYPE_INTERACTABLE ECC_GameTraceChannel3

#define PrintScreenMacro(text) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, text);
#define PrintOnScreenMacroWithKey(key, text) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(key, 10, FColor::Blue, text);
#define PrintScreenMacro_Int(text, num) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("%s %d"),text, num));
#define PrintScreenMacro_Float(text, num) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("%s %f"),text, num));

#define DECLARE_DELEGATE_ADD_REMOVE_FUNC()\
	void BindDelegate(const EDelegateType DelegateType);\
	void RemoveDelegate(const EDelegateType DelegateType);\
	private:\
	TMap<EDelegateType, FDelegateHandle> DelegateMap;

//특정 객체
#define GetInstance_Void(className, variable, worldObject) className* variable = className::Get(worldObject);\
	if(!variable) return;
#define GetInstance_Ret(className, variable, worldObject, ReturnValue) className* variable = className::Get(worldObject);\
	if(!variable) return ReturnValue;
#define DeclareInstanceGetter(className) static className* Get(const UObject* const WorldContextObject);
#define DefineInstanceGetter(className) className* className::Get(const UObject* const WorldContextObject)

//인터페이스 객체
#define GetInterface_Void(interfaceName, className, variable, worldObject) interfaceName* variable = className::Get(worldObject);\
if(!variable) return;
#define GetInterface_Ret(interfaceName, className, worldObject, ReturnValue) className* variable = className::Get(worldObject);\
if(!variable) return ReturnValue;
#define DeclareInterfaceGetter(interfaceName) static interfaceName* Get(const UObject* const WorldContextObject);
#define DefineInterfaceGetter(interfaceName, className) interfaceName* className::Get(const UObject* const WorldContextObject)