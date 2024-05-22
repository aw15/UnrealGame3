// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"
#include "Engine/DataTable.h"
#include "RTStest/GameInstance//GameInstanceBase.h"
#include "RTStest/Common/PredefineMacro.h"

UDataManager::UDataManager()
{
}

DefineInstanceGetter(UDataManager)
{
	GetInstance_Ret(UGameInstanceBase, GameInstance, WorldContextObject, nullptr)
	return GameInstance->DataManager;
}

UDataTable* UDataManager::LoadDataTable(const EDataType DataType)
{
	FString DataResourcePath = TEXT("");
	switch (DataType)
	{
	case EDataType::BuildingUnit: DataResourcePath = TEXT("/Game/Data/Game/DT_BuildingUnit"); break;
	case EDataType::MovableUnit:  DataResourcePath = TEXT("/Game/Data/Game/DT_MovableUnit"); break;
	case EDataType::PlayerColor:  DataResourcePath = TEXT("/Game/Data/Game/DT_PlayerColor"); break;
	case EDataType::ResourceUnit:  DataResourcePath = TEXT("/Game/Data/Game/DT_ResourceUnit"); break;
	}
	const ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(*DataResourcePath);
	if(DataTableObject.Succeeded())
	{
		return DataTableObject.Object;
	}
	return nullptr;
}
