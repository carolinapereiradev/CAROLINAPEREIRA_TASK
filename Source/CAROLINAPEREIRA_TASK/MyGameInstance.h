// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"
#include "MyGameInstance.generated.h"

UCLASS()
class CAROLINAPEREIRA_TASK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetHUDWidget(UUserWidget* WidgetReference) { HUDWidget = Cast<UHUDWidget>(WidgetReference); }

	class UHUDWidget* GetHUDWidget() const { return HUDWidget; }

	void AddCollectibleCollected();

	void IncreaseTotalAmount();

	void UpdateWidgetCollectiblesTotalAmount();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalCollectibleAmount();

protected:
	// Called when the game starts or when spawned
	virtual void Init();

private:
	/* The instance of the HUD widget */
	class UHUDWidget* HUDWidget;

	// Store Game Mode reference
	class AMyGameMode* MyGameMode{};

	int CollectibleAmount = 0;

	int CollectiblesCollected = 0;
};