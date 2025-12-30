// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

// Sets default values
ACollectible::ACollectible()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	FloatingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FloatingTimeline"));

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMeshComponent"));
	CollectibleMesh->SetupAttachment(Root);
	CollectibleMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	CollectibleDetector = CreateDefaultSubobject<USphereComponent>(TEXT("CollectibleSphereDetector"));
	CollectibleDetector->SetSphereRadius(150.f);
	CollectibleDetector->SetupAttachment(CollectibleMesh);
	CollectibleDetector->SetCollisionProfileName(TEXT("Trigger"));
	CollectibleDetector->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlapBegin);
	CollectibleDetector->OnComponentEndOverlap.AddDynamic(this, &ACollectible::OnOverlapEnd);

	// Binding timeline delegators
	FloatingInterpFunction.BindUFunction(this, FName("FloatingTimelineFloatReturn"));
	FloatingTimelineFinished.BindUFunction(this, FName("FloatingOnTimelineFinishes"));
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->IncreaseTotalAmount();
	}

	ObjectStartLocation = GetActorLocation();
	ObjectEndLocation = GetActorLocation() + FVector(0.0f, 0.0f, DistanceToMove);

	// Set parameters for the all the timeline's movement
	if (CollectibleCurveFloat)
	{
		// Add the float curve to the timeline and connect it to the interp function's delegate
		FloatingTimeline->AddInterpFloat(CollectibleCurveFloat, FloatingInterpFunction, FName("Alpha"));
		// Add the on timeline finished function
		FloatingTimeline->SetTimelineFinishedFunc(FloatingTimelineFinished);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CollectibleCurveFloat was not found."));
	}

	if (CollectibleCurveFloat)
	{
		// Defining timeline's settings before starting it
		FloatingTimeline->SetLooping(false);
		FloatingTimeline->SetIgnoreTimeDilation(true);

		// Start timeline
		FloatingTimeline->Play();
	}
}

void ACollectible::FloatingTimelineFloatReturn(float Value)
{
	CollectibleMesh->SetWorldLocation(FMath::Lerp(ObjectStartLocation, ObjectEndLocation, Value));
}

void ACollectible::FloatingOnTimelineFinishes()
{
	if (MovingUp)
	{
		MovingUp = false;

		// Start timeline
		FloatingTimeline->ReverseFromEnd();
	}
	else
	{
		MovingUp = true;

		// Start timeline
		FloatingTimeline->Play();
	}
}

void ACollectible::CollectCollectible()
{
	GameInstance->AddCollectibleCollected();
	Destroy();
}

void ACollectible::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherComp)
	{
		CollectCollectible();
	}
}

void ACollectible::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherComp)
	{
		// Nothing to do here for now
	}
}