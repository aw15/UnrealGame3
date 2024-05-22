// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBase.h"

#include "Kismet/GameplayStatics.h"
#include "RTStest/Data/DataManager.h"
#include "RTStest/GameDefault/RtsGameStateBase.h"
#include "RTStest/SaveSystem/BuildingSaveObject.h"

void UGameInstanceBase::Init()
{
	Super::Init();
	DataManager = NewObject<UDataManager>(this, UDataManager::StaticClass(), TEXT("DataManager"));
	BuildingSaveSystem = NewObject<UBuildingSaveSystem>(this, UBuildingSaveSystem::StaticClass(), TEXT("BuildingSaveSystem"));
}

DefineInstanceGetter(UGameInstanceBase)
{
	return Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(WorldContextObject));
}
