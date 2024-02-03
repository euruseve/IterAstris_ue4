// Iter Astris. All Rights Reserved.

#include "Player/IABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/IACharacterMovementComponent.h"
#include "Weapons/IABaseWeapon.h"
#include "Components/Player/IAPlayerHealthComponent.h"
#include "Components/Player/IAPlayerIntoxicationComponent.h"
#include "Components/IAWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AIABaseCharacter::AIABaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UIACharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    PlayerSuitMode = EPlayerSuitMode::WithoutSuit;

    CameraView = ECameraView::ThirdPersonView;
    BaseTurnRate = 45.f;
    DefaultTargetArmLenght = 300.f;
    CurrentTargetArmLenght = DefaultTargetArmLenght;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = DefaultTargetArmLenght;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    PlayerHealthComponent = CreateDefaultSubobject<UIAPlayerHealthComponent>("PlayerHealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthText");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    PlayerIntoxicationComponent = CreateDefaultSubobject<UIAPlayerIntoxicationComponent>("PlayerIntoxicationComponent");

    IntoxicationTextComponent = CreateDefaultSubobject<UTextRenderComponent>("IntoxicationText");
    IntoxicationTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<UIAWeaponComponent>("WeaponComponent");
}

void AIABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(PlayerHealthComponent);
    check(PlayerIntoxicationComponent);
    check(WeaponComponent);
    check(GetCharacterMovement());

    OnHealthChanged(PlayerHealthComponent->GetHealth());
    PlayerHealthComponent->OnDeath.AddUObject(this, &AIABaseCharacter::OnDeath);
    PlayerHealthComponent->OnHealthChanged.AddUObject(this, &AIABaseCharacter::OnHealthChanged);

    PlayerIntoxicationComponent->OnToxinLevelChanged.AddDynamic(this, &AIABaseCharacter::OnToxinLevelChanged);

    LandedDelegate.AddDynamic(this, &AIABaseCharacter::OnGroundLanded);

    check(PlayerModels.BaseMesh);
    check(PlayerModels.SpaceSuitMesh);

    GetMesh()->SetSkeletalMesh(PlayerModels.BaseMesh);
}

void AIABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AIABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AIABaseCharacter::OnStartJumping);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AIABaseCharacter::OnStopJumping);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AIABaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AIABaseCharacter::OnStopRunning);

    PlayerInputComponent->BindAction("ChangeCameraView", IE_Pressed, this, &AIABaseCharacter::ChangeCameraView);

    PlayerInputComponent->BindAction("ChangeCostumeMode", IE_Pressed, this, &AIABaseCharacter::ChangeCostumeMode);

    PlayerInputComponent->BindAction("EquipWeapon", IE_Pressed, this, &AIABaseCharacter::WeaponMode);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AIABaseCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AIABaseCharacter::StopFire);

    PlayerInputComponent->BindAxis("CameraZoom", this, &AIABaseCharacter::CameraZoom);

    PlayerInputComponent->BindAxis("MoveForward", this, &AIABaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AIABaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &AIABaseCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AIABaseCharacter::LookUpRate);

    PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnAroundRate", this, &AIABaseCharacter::TurnAroundRate);
}

void AIABaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AIABaseCharacter::OnToxinLevelChanged(float OwnToxinLvl)
{
    IntoxicationTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), OwnToxinLvl)));
}

void AIABaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    if (FallVelocityZ < LandedDamageVelocity.X)
        return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

// MOVING
void AIABaseCharacter::Move(float Amount, const FVector& Direction, const EAxis::Type& AxisType)
{
    if (Controller && (Amount != 0.f))
    {
        if (CameraView == ECameraView::ThirdPersonView)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector MovementDirection = FRotationMatrix(YawRotation).GetUnitAxis(AxisType);

            AddMovementInput(MovementDirection, Amount);
        }
        else if (CameraView == ECameraView::FirstPersonView || CameraView == ECameraView::WeaponEquipedView)
        {
            if (Direction == FVector::ForwardVector)
                AddMovementInput(GetActorForwardVector(), Amount);
            else if (Direction == FVector::RightVector)
                AddMovementInput(GetActorRightVector(), Amount);
        }
    }
}

void AIABaseCharacter::MoveForward(float Amount)
{
    Move(Amount, FVector::ForwardVector, EAxis::X);
}

void AIABaseCharacter::MoveRight(float Amount)
{
    Move(Amount, FVector::RightVector, EAxis::Y);
}

void AIABaseCharacter::OnStartRunning()
{
    bWantsToRun = true;
}

void AIABaseCharacter::OnStopRunning()
{
    bWantsToRun = false;
}

bool AIABaseCharacter::IsRunning() const
{
    return bWantsToRun && !GetVelocity().IsZero();
}

float AIABaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AIABaseCharacter::OnStartJumping()
{
    bCanWearCostume = false;
    Jump();
}

void AIABaseCharacter::OnStopJumping()
{
    StopJumping();
    bCanWearCostume = true;
}
//

// CAMERA
void AIABaseCharacter::LookUp(float Amount)
{
    SetCameraViewSettings();

    AddControllerPitchInput(Amount);
}

void AIABaseCharacter::LookUpRate(float Rate)
{
    if (CameraView == ECameraView::ThirdPersonView)
        AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIABaseCharacter::TurnAround(float Amount)
{
    SetCameraViewSettings();

    AddControllerYawInput(Amount);
}

void AIABaseCharacter::TurnAroundRate(float Rate)
{
    if (CameraView == ECameraView::ThirdPersonView)
        AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIABaseCharacter::CameraZoom(float Amount)
{
    if (!bCanCameraMove)
        return;

    if (CameraView == ECameraView::ThirdPersonView)
    {
        ThirdPersonZoom(Amount);
    }
    else if (CameraView == ECameraView::FirstPersonView)
    {
        FirstPersonZoom(Amount);
    }
    else if (CameraView == ECameraView::WeaponEquipedView)
    {
        WeaponModeZoom(Amount);
    }
}

void AIABaseCharacter::FirstPersonZoom(float Amount)
{
    if (Amount < 0)
    {
        CurrentTargetArmLenght = 200.f;
        SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

        FullCameraSettingsReset();
    }
}

void AIABaseCharacter::ThirdPersonZoom(float Amount)
{
    float ZoomLenght = SpringArmComponent->TargetArmLength + (Amount * -10);
    CurrentTargetArmLenght = FMath::Clamp(ZoomLenght, 199.f, 500.f);
    SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

    if (CurrentTargetArmLenght < 200.f)
    {
        CurrentTargetArmLenght = DefaultTargetArmLenght;
        FullCameraSettingsReset();
    }
}

void AIABaseCharacter::WeaponModeZoom(float Amount)
{
    float ZoomLenght = SpringArmComponent->TargetArmLength + (Amount * -10);
    CurrentTargetArmLenght = FMath::Clamp(ZoomLenght, 300.f, 400.f);
    SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;
}

void AIABaseCharacter::ChangeCameraView()
{
    if (!bCanCameraMove)
        return;

    if (CameraView == ECameraView::FirstPersonView)
    {
        CameraView = ECameraView::ThirdPersonView;
        SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

        UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to TP"));
    }
    else if (CameraView == ECameraView::ThirdPersonView)
    {
        CameraView = ECameraView::FirstPersonView;
        SpringArmComponent->TargetArmLength = 0;

        UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to FP"));
    }
}

void AIABaseCharacter::SetCameraViewSettings()
{
    if (!bCanCameraMove)
        return;

    if (CameraView == ECameraView::ThirdPersonView)
    {
        bUseControllerRotationYaw = false;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        // GetMesh()->SetOwnerNoSee(false);
    }
    else if (CameraView == ECameraView::WeaponEquipedView || CameraView == ECameraView::FirstPersonView)
    {
        bUseControllerRotationYaw = true;
        GetCharacterMovement()->bOrientRotationToMovement = false;
        // GetMesh()->SetOwnerNoSee(false);
    }
}

void AIABaseCharacter::FullCameraSettingsReset()
{
    ChangeCameraView();
    SetCameraViewSettings();
}

void AIABaseCharacter::ChangeSocketOffsetY(float Offset)
{
    SpringArmComponent->SocketOffset.Y = Offset;
}
//

void AIABaseCharacter::ChangeCostumeMode()
{
    if (IsRunning() || !bCanWearCostume)
        return;

    if (PlayerSuitMode == EPlayerSuitMode::SpaceSuit)
    {
        PlayerSuitMode = EPlayerSuitMode::WithoutSuit;
        GetMesh()->SetSkeletalMesh(PlayerModels.BaseMesh);
        UE_LOG(LogBaseCharacter, Display, TEXT("Without costume mode"));
    }
    else if (PlayerSuitMode == EPlayerSuitMode::WithoutSuit)
    {
        PlayerSuitMode = EPlayerSuitMode::SpaceSuit;
        GetMesh()->SetSkeletalMesh(PlayerModels.SpaceSuitMesh);
        UE_LOG(LogBaseCharacter, Display, TEXT("Costume mode"));
    }
    PlayAnimMontage(PlayerAnims.SuitModeAnimMintage);
}

// DEATH
void AIABaseCharacter::OnDeath()
{
    OnDeathCameraChange();
    bCanWearCostume = false;

    UE_LOG(LogBaseCharacter, Display, TEXT("DEAD"));

    PlayAnimMontage(PlayerAnims.DeathAnimMintage);
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(10.f);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AIABaseCharacter::OnDeathCameraChange()
{
    bCanCameraMove = false;
    SpringArmComponent->TargetArmLength = 500;
}
//

// WEAPON
void AIABaseCharacter::StartFire()
{
    if (bHasWeapon)
        WeaponComponent->StartFire();
}

void AIABaseCharacter::StopFire()
{
    WeaponComponent->StopFire();
}

void AIABaseCharacter::WeaponMode()
{
    bHasWeapon = !bHasWeapon;

    if (bHasWeapon)
    {
        ChangeSocketOffsetY(75.f);
        EquipWeapon();
    }
    else
    {
        ChangeSocketOffsetY(0.f);
        UnequipWeapon();
    }

    UE_LOG(LogBaseCharacter, Display, TEXT("HasWeapon is: %s"), (bHasWeapon ? TEXT("TRUE") : TEXT("FALSE")));
}

void AIABaseCharacter::EquipWeapon()
{
    PlayAnimMontage(PlayerAnims.EquipWeaponAnimMintage);

    CameraView = ECameraView::WeaponEquipedView;

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponComponent->SetWeapon("RWeaponSocket"); }, 1.0f, false);
}

void AIABaseCharacter::UnequipWeapon()
{
    PlayAnimMontage(PlayerAnims.UnequipWeaponAnimMintage);

    CameraView = ECameraView::ThirdPersonView;

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponComponent->SetWeapon("SpineWeaponSocket"); }, 2.0f, false);
}

//
