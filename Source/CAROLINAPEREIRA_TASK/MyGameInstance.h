// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class CAROLINAPEREIRA_TASK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetHUDWidget(class UUserWidget* WidgetReference) { HUDWidget = WidgetReference; }

	class UUserWidget* GetHUDWidget() { return HUDWidget; }

	void AddCollectible();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCollectibleAmount();

	UFUNCTION(BlueprintCallable)
	void SetCollectiblesAmount(int Amount);

protected:
	// Called when the game starts or when spawned
	virtual void Init();

private:

	/* The instance of the HUD widget */
	class UUserWidget* HUDWidget;

	// Store Game Mode reference
	class AMyGameMode* MyGameMode{};

	int CollectibleAmount = 0;
};