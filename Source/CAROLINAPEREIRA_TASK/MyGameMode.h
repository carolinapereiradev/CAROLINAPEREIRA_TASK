#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

class UUserWidget;
class UMyGameInstance;

UCLASS()
class CAROLINAPEREIRA_TASK_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

protected:
	virtual void BeginPlay() override;

	/* The HUD widget class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget = nullptr;

	UPROPERTY()
	UMyGameInstance* MyGameInstance = nullptr;
};
