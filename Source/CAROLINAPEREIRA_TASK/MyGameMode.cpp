#include "MyGameMode.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (!MyGameInstance) return;

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			if (MyGameInstance->GetHUDWidget() == nullptr)
			{
				MyGameInstance->SetHUDWidget(HUDWidget);
				MyGameInstance->UpdateWidgetCollectiblesTotalAmount();
			}
		}
	}
}
