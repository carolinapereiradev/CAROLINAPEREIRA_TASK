// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "HUDWidget.h"
#include "MyGameMode.h"

// Called when the game starts or when spawned
void UMyGameInstance::Init()
{
	// Set the default game mode class to MyGameMode
	if (GetWorld())
	{
		MyGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}
}

void UMyGameInstance::AddCollectibleCollected()
{
	CollectiblesCollected += 1;

	if (HUDWidget)
	{
		HUDWidget->UpdateCollectiblesCollected(CollectiblesCollected);
	}
}

void UMyGameInstance::IncreaseTotalAmount()
{
	CollectibleAmount += 1;	
}

void UMyGameInstance::UpdateWidgetCollectiblesTotalAmount()
{
	if (HUDWidget)
	{
		HUDWidget->SetTotalAmount(CollectibleAmount);
	}
}

int UMyGameInstance::GetTotalCollectibleAmount()
{
	return CollectibleAmount;
}