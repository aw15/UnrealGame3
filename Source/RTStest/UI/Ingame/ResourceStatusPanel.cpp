// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceStatusPanel.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Pawns/PlayerStateBase.h"

void UResourceStatusPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	StoneText = Cast<UTextBlock>(GetWidgetFromName("StoneText"));
	WoodText = Cast<UTextBlock>(GetWidgetFromName("WoodText"));
	GoldText = Cast<UTextBlock>(GetWidgetFromName("GoldText"));

	if(StoneText && WoodText && GoldText)
	{
		bUiInitialized = true;
		GetWorld()->GetTimerManager().SetTimer(UpdateHandle, this, &UResourceStatusPanel::UpdateResource, UPDATE_TIME, true);
	}
}

void UResourceStatusPanel::UpdateResource()
{
	GetInstance_Void(APlayerStateBase, PlayerStateBase, GetWorld())
	if(false == bUiInitialized)
		return;
	
	StoneText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStateBase->StoneCount)));
	WoodText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStateBase->WoodCount)));
	GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStateBase->GoldCount)));
}
