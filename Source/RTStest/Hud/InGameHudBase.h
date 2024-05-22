// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTStest/Common/EngineEventInterface.h"
#include "RTStest/Common/PredefineEnum.h"

#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/UI/Confirm/ConfirmOwner.h"
#include "RTStest/UI/Confirm/UiInterface.h"
#include "RTStest/UI/Ingame/CommonUnitMenuBase.h"
#include "InGameHudBase.generated.h"

class AMovableUnit;
/**
 * 
 */
UCLASS()
class RTSTEST_API AInGameHudBase : public AHUD, public IEngineEventListener
{
	GENERATED_BODY()
public:
	DeclareInstanceGetter(AInGameHudBase)
	UFUNCTION(BlueprintCallable)
	virtual void ShowMenu(TSubclassOf<UCommonUnitMenuBase> ClassRef, const bool bShow);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnClickBuildingSlot(const class ABuildingSlot* BuildingSlot);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenConfirmPanel(const EConfirmPanelType ConfirmPanelType, const TScriptInterface<IConfirmOwner>& ConfirmOwner);

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category="Unit")
	TArray<AActor*> SelectedActors;
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category="Unit")
	UCommonUnitMenuBase* CommonUnitMenu;
};

