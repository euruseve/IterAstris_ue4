// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IABaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UIAPlayerHealthComponent;
class UIAPlayerIntoxicationComponent;
class AIABaseWeapon;

UENUM(BlueprintType)
enum class ECameraView : uint8
{
    FirstPersonView UMETA(DisplayName = "FirstPerson"),
    ThirdPersonView UMETA(DisplayName = "ThirdPerson")
};

UENUM(BlueprintType)
enum class EPlayerSuitMode : uint8
{
    WithoutSuit UMETA(DisplayName = "WithoutSuit"),
    SpaceSuit UMETA(DisplayName = "SpaceSuit")
};

USTRUCT(BlueprintType)
struct FPlayerModels
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = "Models")
    USkeletalMesh* BaseMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Models")
    USkeletalMesh* SpaceSuitMesh;
};

UCLASS()
class ITERASTRIS_API AIABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AIABaseCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Models")
    FPlayerModels PlayerModels;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    EPlayerSuitMode PlayerSuitMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAPlayerHealthComponent* PlayerHealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAPlayerIntoxicationComponent* PlayerIntoxicationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* IntoxicationTextComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    ECameraView CameraView;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float BaseTurnRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float DefaultTargetArmLenght;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Options")
    float CurrentTargetArmLenght;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SuitModeAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity{1200.f, 1500.f};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage{10.f, 80.f};

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AIABaseWeapon> Weapon;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsWeapon() const { return bHasWeapon; };

    bool IsPlayerInCostume() const;

private:
    bool bWantsToRun = false;
    bool bCanWearCostume = true;
    bool bCanCameraMove = true;
    bool bHasWeapon = false;

    UFUNCTION()
    void OnToxinLevelChanged(float OwnToxinLvl);
    void OnHealthChanged(float Health);
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    void Move(float Amount, const FVector& Direction, const EAxis::Type& AxisType);
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void OnStartRunning();
    void OnStopRunning();

    void OnStartJumping();
    void OnStopJumping();

    void TurnAround(float Amount);
    void LookUp(float Amount);

    void TurnAroundRate(float Rate);
    void LookUpRate(float Rate);

    void CameraZoom(float Amount);
    void ChangeCameraView();
    void SetCameraViewSettings();

    void FullCameraSettingsReset();

    void ChangeCostumeMode();

    void OnDeath();
    void OnDeathCameraChange();


    void EquipWeapon();
    void SpawnWeapon();
};
