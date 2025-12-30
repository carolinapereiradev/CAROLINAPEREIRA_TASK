#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

UCLASS()
class CAROLINAPEREIRA_TASK_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Function to update the collectible count
	UFUNCTION(BlueprintCallable, Category = "HUD")
	virtual void UpdateCollectiblesCollected(int NewAmount);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	virtual void SetTotalAmount(int NewAmount);

protected:
	// Bind the TextBlock from the Blueprint
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CollectiblesCollectedText;

	// Bind the TextBlock from the Blueprint
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalCollectibles;
};
