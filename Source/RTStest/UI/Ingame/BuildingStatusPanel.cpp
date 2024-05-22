// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingStatusPanel.h"

#include "RTStest/UI/UIPredifines.h"

UBuildingStatusPanel::UBuildingStatusPanel(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UBuildingStatusPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildingStatusPanel::Update_Implementation(ABuildingSlot* BuildingSlot)
{
}
