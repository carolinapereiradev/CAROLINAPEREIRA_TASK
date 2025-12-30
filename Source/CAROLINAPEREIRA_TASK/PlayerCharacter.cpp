// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create mesh for the skateboard
	Skate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateStaticMeshComponent"));
	SkateBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateBaseStaticMeshComponent"));
	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelStaticMeshComponent"));
	Wheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelsStaticMeshComponent"));

	// Attach skateboard components to the character mesh
	Skate->SetupAttachment(GetMesh());
	SkateBase->SetupAttachment(Skate);
	Wheel->SetupAttachment(Skate);
	Wheels->SetupAttachment(Skate);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeSpeed(MinSpeed);
	SpeedWithImpulse = MinSpeed;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Camera
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::TryToJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &APlayerCharacter::Push);
}

void APlayerCharacter::MoveForward(float Value)
{
	// If we're already at rest AND no input is given, do nothing
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		MovementValue = FMath::FInterpTo(MovementValue,Value, GetWorld()->GetDeltaSeconds(), 2.0f);
		AddMovementInput(Direction, MovementValue);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		TurningRateValue = FMath::FInterpTo(TurningRateValue, Value, GetWorld()->GetDeltaSeconds(), 2.0f);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::TryToJump()
{
	Jump();
}

void APlayerCharacter::Push()
{
	if (!isPushing && !GetCharacterMovement()->IsFalling())
	{
		float NewSpeed = SpeedWithImpulse + PushStrength;

		if (NewSpeed <= MaxSpeed)
		{
			SpeedWithImpulse = NewSpeed;
			ChangeSpeed(SpeedWithImpulse);
		}
		else
		{
			SpeedWithImpulse = MaxSpeed;
			ChangeSpeed(SpeedWithImpulse);
		}

		isTryingToPush = true;
		isPushing = true;

		// Schedule reset
		GetWorldTimerManager().SetTimer(PushCooldownTimer, this, &APlayerCharacter::ResetPush, PushCooldown, false);
	}
}

void APlayerCharacter::ResetPush()
{
	isPushing = false;
}

void APlayerCharacter::ChangeSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Gradually reduce speed back to minSpeed speed after a push
	if (SpeedWithImpulse > MinSpeed)
	{
		SpeedWithImpulse -= DecayRate * DeltaTime;

		// Never let SpeedWithImpulse go below MinSpeed
		SpeedWithImpulse = FMath::Max(SpeedWithImpulse, MinSpeed);

		ChangeSpeed(SpeedWithImpulse);
	}

	if (GetVelocity().Length() <= 400)
	{
		ChangeSpeed(MinSpeed);
		SpeedWithImpulse = MinSpeed;
	}
}

