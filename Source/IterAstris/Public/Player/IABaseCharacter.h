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
class UIAWeaponComponent;
class UBoxComponent;


UENUM(BlueprintType)
enum class EViewMode : uint8
{
    WeaponEquipedView UMETA(DisplayName = "Weapon Equiped"),
    DialogueView UMETA(DisplayName = "Dialogue"),
    FirstPersonView UMETA(DisplayName = "First Person"),
    ThirdPersonView UMETA(DisplayName = "Third Person")
};

UENUM(BlueprintType)
enum class EPlayerSuitMode : uint8
{
    WithoutSuit UMETA(DisplayName = "Without Suit"),
    SpaceSuit UMETA(DisplayName = "Space Suit")
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

USTRUCT(BlueprintType)
struct FPlayerAnimations
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SuitModeAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipWeaponAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* UnequipWeaponAnimMintage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ShootAnimMintage;
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
    
    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    FPlayerAnimations PlayerAnims;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    EPlayerSuitMode PlayerSuitMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* ThirdPersonCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UCameraComponent* FirstPersonCamera; 
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UCameraComponent* DialogueCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAPlayerHealthComponent* PlayerHealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAPlayerIntoxicationComponent* PlayerIntoxicationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* IntoxicationTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    EViewMode CameraView;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float BaseTurnRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Options")
    float DefaultTargetArmLenght;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Options")
    float CurrentTargetArmLenght;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interection Box")
    UBoxComponent* InteractionBox;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity{1200.f, 1500.f};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage{10.f, 80.f};

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const; 
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;


    UFUNCTION(BlueprintCallable, Category = "Player Mode")
    bool IsWeaponEquiped() const { return bHasWeapon; };    

    UFUNCTION(BlueprintCallable, Category = "Player Mode")
    bool IsPlayerInCostume() const { return PlayerSuitMode == EPlayerSuitMode::SpaceSuit; };


    UFUNCTION(BlueprintCallable, Category = "Weapon Charge")
    bool IsWeaponCharging() const { return bIsWeaponCharging; }; 
    
    UFUNCTION(BlueprintCallable, Category = "Weapon Charge")
    void SetWeaponCharging(bool Charging) { bIsWeaponCharging = Charging; };

    UFUNCTION(BlueprintCallable, Category = "Component")
    UIAWeaponComponent* GetWeaponComponent() { return WeaponComponent; };

private:
    bool bWantsToRun = false;
    bool bCanWearCostume = true;
    bool bCanCameraMove = true;
    bool bHasWeapon = false;
    bool bCanShot = false;
    bool bCanInteract = true;
    bool bAnimationInProgress = false;
    bool bIsWeaponCharging = false;

    float WeaponEquipTime = 1.f;
    float WeaponUnequipTime = 2.f;

    AActor* Interactable = nullptr; 
    UFUNCTION()
    void OnInteractionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnInteractionBoxEndOverlap(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void OnInteract();
    void GetClosestInterctableObject();

    AActor* SpawnedWeapon = nullptr;

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
    void FirstPersonZoom(float Amount);
    void ThirdPersonZoom(float Amount);
    void WeaponModeZoom(float Amount);
    void ChangeSocketOffsetY(float Offset);
    void ChangeCameraView();
    void SetCameraViewSettings();

    void FullCameraSettingsReset();

    void ChangeCostumeMode();

    void OnDeath();
    void OnDeathCameraChange();

    void StartFire();
    void StopFire();

    void WeaponMode();
    void EquipWeapon();
    void UnequipWeapon();

    void InitAnimations();
    void OnSuitChange(USkeletalMeshComponent* MeshComp);
    void OnWeaponEquiped(USkeletalMeshComponent* MeshComp);
};
