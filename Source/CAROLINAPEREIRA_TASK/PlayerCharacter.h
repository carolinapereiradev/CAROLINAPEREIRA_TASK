#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CAROLINAPEREIRA_TASK_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayerCharacter();

    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    UFUNCTION(BlueprintCallable, Category = "Skate")
    bool PushState() { return isTryingToPush; }

    UFUNCTION(BlueprintCallable, Category = "Skate")
    void ChangePushState (bool NewState) { isTryingToPush = NewState; }

    UFUNCTION(BlueprintCallable, Category = "Skate")
    void ResetPush();

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    FTimerHandle PushCooldownTimer;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    /** Skate components */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skate, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Skate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skate, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* SkateBase;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skate, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Wheel;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skate, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Wheels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skate")
    float PushCooldown = 1.2f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MinSpeed = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Skate")
    float PushStrength = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Skate")
    float MaxSpeed = 1800.0f;

    UPROPERTY(EditAnywhere, Category = "Skate")
    float DecayRate = 40.0f;

    /** Current movement velocity and input direction */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector CurrentVelocity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector InputDirection;

    bool isTryingToPush = false;

    bool isPushing = false;

    float MovementValue = 0.f;

	float TurningRateValue = 0.f;

    float SpeedWithImpulse = 0.0f;

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /** Called for skate impulse */
    void Push();

    void ChangeSpeed(float Speed);

    void TryToJump();

    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
};
