// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Data/UnitData.h"
#include "RTStest/Pawns/Interface/UnitInterface.h"
#include "AutoControlledPlayer.generated.h"

class ABuildingSlot;
/**
 * 
 */
UCLASS(Blueprintable)
class RTSTEST_API UAutoControlledPlayer : public UActorComponent
{
	GENERATED_BODY()
public:
	DeclareInstanceGetter(UAutoControlledPlayer);
	UAutoControlledPlayer();
	virtual void BeginPlay() override;
	void SetResource(const EResourceType ResourceType, const int Amount);

protected:
	void OnBuildingLoadCompleted();
	void RefreshStrategy();

private:
	void OnBuildingDestroyed(const TScriptInterface<IHasOwnerActor>&);

protected:
	UPROPERTY(VisibleAnywhere, Category="Need Save")
	int GoldCount;
	UPROPERTY(VisibleAnywhere, Category="Need Save")
	int WoodCount;
	UPROPERTY(VisibleAnywhere, Category="Need Save")
	int StoneCount;
private:
	TSet<ABuildingSlot*> AllPlayerBuildingSlots;
	TSet<ABuildingSlot*> AllEnemyBuildingSlots;
	FTimerHandle StrategyRefreshTimerHandle;
};
