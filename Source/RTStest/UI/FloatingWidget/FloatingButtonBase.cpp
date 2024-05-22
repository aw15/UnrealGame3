// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingButtonBase.h"

#include "Components/Button.h"

void UFloatingButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonRef = Cast<UButton>(GetWidgetFromName("MainButton"));
	if(ButtonRef)
	{
		ButtonRef->OnClicked.AddDynamic(this, &UFloatingButtonBase::OnClicked);
	}
}

void UFloatingButtonBase::OnClicked()
{
	OnClickBtn.ExecuteIfBound();
}
