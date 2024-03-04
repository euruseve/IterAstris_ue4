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
#include "Components/BoxComponent.h"
#include "Animations/IASuitModeChangeAnimNotify.h"
#include "Animations/IAWeaponEquipFinishAnimNotify.h"
#include <Interfaces/InteractionInterface.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AIABaseCharacter::AIABaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UIACharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    PlayerSuitMode = EPlayerSuitMode::WithoutSuit;

    CameraView = EViewMode::ThirdPersonView;
    BaseTurnRate = 45.f;
    DefaultTargetArmLenght = 300.f;
    CurrentTargetArmLenght = DefaultTargetArmLenght;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = DefaultTargetArmLenght;

    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCameraComponent");
    ThirdPersonCamera->SetupAttachment(SpringArmComponent);

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCameraComponent");
    FirstPersonCamera->SetupAttachment(GetMesh(), "HeadSocket");

    DialogueCamera = CreateDefaultSubobject<UCameraComponent>("DialogueCameraComponent");
    DialogueCamera->SetupAttachment(GetRootComponent());

    PlayerHealthComponent = CreateDefaultSubobject<UIAPlayerHealthComponent>("PlayerHealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthText");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    PlayerIntoxicationComponent = CreateDefaultSubobject<UIAPlayerIntoxicationComponent>("PlayerIntoxicationComponent");

    IntoxicationTextComponent = CreateDefaultSubobject<UTextRenderComponent>("IntoxicationText");
    IntoxicationTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<UIAWeaponComponent>("WeaponComponent");

    InteractionBox = CreateDefaultSubobject<UBoxComponent>("InteractionBoxComponent");
    InteractionBox->SetupAttachment(GetRootComponent());
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

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AIABaseCharacter::OnInteractionBoxBeginOverlap);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AIABaseCharacter::OnInteractionBoxEndOverlap);

    check(PlayerModels.BaseMesh);
    check(PlayerModels.SpaceSuitMesh);

    GetMesh()->SetSkeletalMesh(PlayerModels.BaseMesh);

    InitAnimations();
}

void AIABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    GetClosestInterctableObject();
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

    PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AIABaseCharacter::OnInteract);

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
    if (Controller && (Amount != 0.f) && !bAnimationInProgress)
    {
        if (CameraView == EViewMode::ThirdPersonView)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector MovementDirection = FRotationMatrix(YawRotation).GetUnitAxis(AxisType);

            AddMovementInput(MovementDirection, Amount);
        }
        else if (CameraView == EViewMode::FirstPersonView || CameraView == EViewMode::WeaponEquipedView)
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

// CAMERA INPUT
void AIABaseCharacter::LookUp(float Amount)
{
    SetCameraViewSettings();

    AddControllerPitchInput(Amount);
}

void AIABaseCharacter::LookUpRate(float Rate)
{
    if (CameraView == EViewMode::ThirdPersonView)
        AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIABaseCharacter::TurnAround(float Amount)
{
    SetCameraViewSettings();

    AddControllerYawInput(Amount);
}

void AIABaseCharacter::TurnAroundRate(float Rate)
{
    if (CameraView == EViewMode::ThirdPersonView)
        AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// CAMERA ZOOM
void AIABaseCharacter::CameraZoom(float Amount)
{
    if (!bCanCameraMove)
        return;

    if (CameraView == EViewMode::ThirdPersonView)
    {
        ThirdPersonZoom(Amount);
    }
    else if (CameraView == EViewMode::FirstPersonView)
    {
        FirstPersonZoom(Amount);
    }
    else if (CameraView == EViewMode::WeaponEquipedView)
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

// CAMERA GLOBAL
void AIABaseCharacter::ChangeCameraView()
{
    if (!bCanCameraMove)
        return;

    if (CameraView == EViewMode::FirstPersonView)
    {
        CameraView = EViewMode::ThirdPersonView;

        FirstPersonCamera->SetActive(false);
        ThirdPersonCamera->SetActive(true);

        SpringArmComponent->TargetArmLength = CurrentTargetArmLenght;

        // UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to TP"));
    }
    else if (CameraView == EViewMode::ThirdPersonView)
    {
        CameraView = EViewMode::FirstPersonView;

        ThirdPersonCamera->SetActive(false);
        FirstPersonCamera->SetActive(true);

        // UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to FP"));
    }
}

void AIABaseCharacter::SetCameraViewSettings()
{
    if (!bCanCameraMove)
        return;

    if (CameraView == EViewMode::ThirdPersonView)
    {
        bUseControllerRotationYaw = false;
        GetCharacterMovement()->bOrientRotationToMovement = true;
    }
    else if (CameraView == EViewMode::FirstPersonView || CameraView == EViewMode::WeaponEquipedView)
    {
        bUseControllerRotationYaw = true;
        GetCharacterMovement()->bOrientRotationToMovement = false;
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

    bAnimationInProgress = true;
    bCanShot = false;

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
    bCanShot = false;

    // UE_LOG(LogBaseCharacter, Display, TEXT("DEAD"));

    PlayAnimMontage(PlayerAnims.DeathAnimMintage);
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.f);
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
    if (bHasWeapon && bCanShot )
    {
        bCanWearCostume = false;
        // PlayAnimMontage(PlayerAnims.ShootAnimMintage);
        WeaponComponent->StartFire();
    }
}

void AIABaseCharacter::StopFire()
{
    WeaponComponent->StopFire();
    bCanWearCostume = true;
}

void AIABaseCharacter::WeaponMode()
{
    if (bAnimationInProgress || bIsWeaponCharging)
        return;

    if (CameraView == EViewMode::FirstPersonView)
        ChangeCameraView();

    bHasWeapon = !bHasWeapon;
    bCanShot = false;

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
    bAnimationInProgress = true;

    PlayAnimMontage(PlayerAnims.EquipWeaponAnimMintage);

    CameraView = EViewMode::WeaponEquipedView;

    WeaponComponent->ShowWeapon();

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponComponent->SetWeapon("RWeaponSocket"); }, WeaponEquipTime, false);
}

void AIABaseCharacter::UnequipWeapon()
{
    bAnimationInProgress = true;

    PlayAnimMontage(PlayerAnims.UnequipWeaponAnimMintage);

    CameraView = EViewMode::ThirdPersonView;

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponComponent->SetWeapon("SpineWeaponSocket"); }, WeaponUnequipTime, false);

    WeaponComponent->HideWeapon();
}

//

// NOTIFIES
void AIABaseCharacter::InitAnimations()
{
    if (!PlayerAnims.SuitModeAnimMintage || !PlayerAnims.EquipWeaponAnimMintage ||
        !PlayerAnims.UnequipWeaponAnimMintage)
        return;

    // Costume
    const auto SuitNotifyEvents = PlayerAnims.SuitModeAnimMintage->Notifies;
    for (auto Event : SuitNotifyEvents)
    {
        auto SuitChangeNotif = Cast<UIASuitModeChangeAnimNotify>(Event.Notify);
        if (SuitChangeNotif)
        {
            SuitChangeNotif->OnSuitNotified.AddUObject(this, &AIABaseCharacter::OnSuitChange);
            break;
        }
    }

    // Weapon Equip
    const auto WeaponEqNotifyEvents = PlayerAnims.EquipWeaponAnimMintage->Notifies;
    for (auto Event : WeaponEqNotifyEvents)
    {
        auto WeaponEqNotif = Cast<UIAWeaponEquipFinishAnimNotify>(Event.Notify);
        if (WeaponEqNotif)
        {
            WeaponEqNotif->OnWeaponNotified.AddUObject(this, &AIABaseCharacter::OnWeaponEquiped);
            break;
        }
    }

    // Weapon Unequip
    const auto WeaponUneqNotifyEvents = PlayerAnims.UnequipWeaponAnimMintage->Notifies;
    for (auto Event : WeaponUneqNotifyEvents)
    {
        auto WeaponUneqNotif = Cast<UIAWeaponEquipFinishAnimNotify>(Event.Notify);
        if (WeaponUneqNotif)
        {
            WeaponUneqNotif->OnWeaponNotified.AddUObject(this, &AIABaseCharacter::OnWeaponEquiped);
            break;
        }
    }
}

void AIABaseCharacter::OnSuitChange(USkeletalMeshComponent* MeshComp)
{
    bAnimationInProgress = false;
    bCanShot = true;
}

void AIABaseCharacter::OnWeaponEquiped(USkeletalMeshComponent* MeshComp)
{

    if (GetMesh() == MeshComp)
    {
        bAnimationInProgress = false;
        bCanShot = true;
    }
}
//

// INTERACTION
void AIABaseCharacter::OnInteractionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Interactable = OtherActor;
}

void AIABaseCharacter::OnInteractionBoxEndOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Interactable)
    {
        if (Interactable->Implements<UInteractionInterface>())
            IInteractionInterface::Execute_HideInteractionWidget(Interactable);

        Interactable = nullptr;
    }
}

void AIABaseCharacter::OnInteract()
{
    if (Interactable && bCanInteract)
    {
        if (Interactable->Implements<UInteractionInterface>() && !bAnimationInProgress)
        {
            if (bHasWeapon)
            {
                WeaponMode();

                FTimerHandle TimerHandle;
                GetWorldTimerManager().SetTimer(
                    TimerHandle, [this]() { IInteractionInterface::Execute_Interact(Interactable); },
                    WeaponUnequipTime + 1.f, false);
            }
            else
                IInteractionInterface::Execute_Interact(Interactable);
        }
    }
}

void AIABaseCharacter::GetClosestInterctableObject()
{
    if (!Interactable)
        return;

    TArray<AActor*> OverlappingActors;

    InteractionBox->GetOverlappingActors(OverlappingActors);

    if (OverlappingActors.Num() == 0)
    {
        IInteractionInterface::Execute_HideInteractionWidget(Interactable);
        Interactable = nullptr;
        return;
    }

    AActor* ClosestActor = OverlappingActors[0];

    for (auto CurrentActor : OverlappingActors)
    {
        if (GetDistanceTo(CurrentActor) < GetDistanceTo(ClosestActor))
        {
            ClosestActor = CurrentActor;
        }
    }

    if (Interactable->Implements<UInteractionInterface>())
    {
        IInteractionInterface::Execute_HideInteractionWidget(Interactable);

        Interactable = ClosestActor;

        IInteractionInterface::Execute_ShowInteractionWidget(Interactable);
    }
}
//