// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
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

void UMyGameInstance::AddCollectible()
{
	CollectibleAmount += 1;
}

int UMyGameInstance::GetCollectibleAmount()
{
	return CollectibleAmount;
}

void UMyGameInstance::SetCollectiblesAmount(int Amount)
{
	CollectibleAmount = Amount;
}