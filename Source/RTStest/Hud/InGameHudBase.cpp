// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHudBase.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/Pawns/PlayerStateBase.h"
#include "RTStest/Pawns/Unit/MovableUnit.h"

DefineInstanceGetter(AInGameHudBase)
{
	GetInstance_Ret(APlayerControllerBase, PlayerController, WorldContextObject, nullptr);
	return Cast<AInGameHudBase>(PlayerController->GetHUD());
}
void AInGameHudBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameHudBase::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHudBase::ShowMenu(TSubclassOf<UCommonUnitMenuBase> ClassRef, const bool bShow)
{
	if(!CommonUnitMenu)
	{
		CommonUnitMenu = Cast<UCommonUnitMenuBase>(CreateWidget(GetOwningPlayerController(), ClassRef));
		CommonUnitMenu->AddToViewport(999);
	}
	
	CommonUnitMenu->Show(bShow);
}