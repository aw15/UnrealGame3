// Fill out your copyright notice in the Description page of Project Settings.


#include "RTStest/UI/Confirm/ConfirmPanelBase.h"

void UConfirmPanelBase::OpenConfirmPanel_Implementation(const TScriptInterface<IConfirmOwner>& InConfirmOwner)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ConfirmOwner = InConfirmOwner;
}
