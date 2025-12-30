// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Collectible.generated.h"

UCLASS()
class CAROLINAPEREIRA_TASK_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

	// Called to destroy the collectible and send its data to the HUD
	virtual void CollectCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The reference of the curve of the movement timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Timeline")
	UCurveFloat* CollectibleCurveFloat{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible Properties")
	float DistanceToMove{};

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root{};

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* CollectibleMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollectibleDetector{};

	UMyGameInstance* GameInstance{};

	// Declaring the delegate function to be binded with FloatingTimelineFloatReturn
	FOnTimelineFloat FloatingInterpFunction{};

	// Declaring the delegate function to be binded with FloatingOnTimelineFinishes
	FOnTimelineEvent FloatingTimelineFinished{};

	UTimelineComponent* FloatingTimeline{};

	FVector ObjectStartLocation{};
	FVector ObjectEndLocation{};

	bool MovingUp = true;

	// Called while the timeline is being updated.
	UFUNCTION()
	void FloatingTimelineFloatReturn(float Value);

	// Called when the timeline finishes.
	UFUNCTION()
	void FloatingOnTimelineFinishes();

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
