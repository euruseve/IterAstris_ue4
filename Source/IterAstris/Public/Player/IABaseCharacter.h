// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IABaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class ECameraView : uint8
{
    FirstPersonView UMETA(DisplayName = "FirstPerson"),
    ThirdPersonView UMETA(DisplayName = "ThirdPerson")
};

UCLASS()
class ITERASTRIS_API AIABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AIABaseCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    ECameraView CameraView;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float BaseTurnRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float DefaultTargetArmLenght;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Options")
    float CurrentTargetArmLenght;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Options")
    FVector DefaultCameraOffset;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void Move(float Amount, const FVector& Direction, const EAxis::Type& AxisType);
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void TurnAround(float Amount);
    void LookUp(float Amount);

    void TurnAroundRate(float Rate);
    void LookUpRate(float Rate);

    void CameraZoom(float Amount);
    void ChangeCameraView();
    void SetCameraViewSettings();

    void FullCameraSettingsReset();
};
