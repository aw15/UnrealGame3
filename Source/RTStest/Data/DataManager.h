#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "RTStest/Common/PredefineMacro.h"
#include "DataManager.generated.h"
class UDataTable;

enum class EDataType
{
	ResourceUnit,
	MovableUnit,
	BuildingUnit,
	PlayerColor
};

UCLASS()
class UDataManager : public UObject
{
	GENERATED_BODY()
public:
	UDataManager();
	DeclareInstanceGetter(UDataManager)
	static UDataTable* LoadDataTable(const EDataType DataType);
	DECLARE_DYNAMIC_DELEGATE(FOnDataLoaded);
	//Unit
	UPROPERTY()
	UDataTable* ResourceUnitDataTable;
	UPROPERTY()
	UDataTable* MovableUnitDataTable;
	UPROPERTY()
	UDataTable* BuildingUnitDataTable;
	//Player
	UPROPERTY()
	UDataTable* PlayerColorDataTable;
};