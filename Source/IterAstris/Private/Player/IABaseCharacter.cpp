// Iter Astris. All Rights Reserved.

#include "Player/IABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/IACharacterMovementComponent.h"
#include "Weapons/IABaseWeapon.h"
#include "Components/Player/IAPlayerHealthComponent.h"
#include "Components/Player/IAPlayerIntoxicationComponent.h"

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
}

void AIABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(PlayerHealthComponent);
    check(PlayerIntoxicationComponent);
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

    PlayerInputComponent->BindAction("EquipWeapon", IE_Pressed, this, &AIABaseCharacter::EquipWeapon);

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
        else if (CameraView == ECameraView::FirstPersonView)
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
        float ZoomLenght = SpringArmComponent->TargetArmLength + (Amount * -10);
        CurrentTargetArmLenght = FMath::Clamp(ZoomLenght, 199.f, 500.f);
        SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

        if (CurrentTargetArmLenght < 200.f)
        {
            CurrentTargetArmLenght = DefaultTargetArmLenght;
            FullCameraSettingsReset();
        }
    }
    else if (CameraView == ECameraView::FirstPersonView)
    {
        if (Amount < 0)
        {
            CurrentTargetArmLenght = 200.f;
            SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

            FullCameraSettingsReset();
        }
    }
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
    else if (CameraView == ECameraView::FirstPersonView)
    {
        bUseControllerRotationYaw = true;
        GetCharacterMovement()->bOrientRotationToMovement = false;
        // GetMesh()->SetOwnerNoSee(true);
    }
}

void AIABaseCharacter::FullCameraSettingsReset()
{
    ChangeCameraView();
    SetCameraViewSettings();
}


void AIABaseCharacter::ChangeCostumeMode()
{
    if (IsRunning() || !bCanWearCostume)
        return;

    PlayAnimMontage(SuitModeAnimMintage);

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
}

bool AIABaseCharacter::IsPlayerInCostume() const
{
    return PlayerSuitMode == EPlayerSuitMode::SpaceSuit;
}


void AIABaseCharacter::OnDeath()
{
    bCanWearCostume = false;
    bCanCameraMove = false;
    OnDeathCameraChange();

    UE_LOG(LogBaseCharacter, Display, TEXT("DEAD"));
    PlayAnimMontage(DeathAnimMintage);
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(10.f);
}

void AIABaseCharacter::OnDeathCameraChange()
{
    CameraView = ECameraView::ThirdPersonView;
    SpringArmComponent->TargetArmLength = 450;
}


void AIABaseCharacter::EquipWeapon() 
{
    bHasWeapon = !bHasWeapon;

    if (bHasWeapon)
    {
        SpawnWeapon();
    }
    else
    {
        DestroyActor()
    }

    UE_LOG(LogBaseCharacter, Display, TEXT("HasWeapon is: %s"), (bHasWeapon ? TEXT("TRUE") : TEXT("FALSE")));
}

void AIABaseCharacter::SpawnWeapon() 
{
    if (!GetWorld())
        return;

    const auto WeaponObj = GetWorld()->SpawnActor<AIABaseWeapon>(Weapon);
    if (WeaponObj)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
        WeaponObj->AttachToComponent(GetMesh(), AttachmentRules, "RWeaponSocket");
    }
}

