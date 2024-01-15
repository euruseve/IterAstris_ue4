﻿// Iter Astris. All Rights Reserved.

#include "Player/IABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AIABaseCharacter::AIABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // bUseControllerRotationYaw = false;
    // GetCharacterMovement()->bOrientRotationToMovement = true;

    CameraView = ECameraView::ThirdPersonView;
    BaseTurnRate = 45.f;
    DefaultTargetArmLenght = 300.f;
    CurrentTargetArmLenght = DefaultTargetArmLenght;

    // TODO взнати що воно робить
    // GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    // GetCharacterMovement()->JumpZVelocity = 600.f;
    // GetCharacterMovement()->AirControl = 0.2f;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = DefaultTargetArmLenght;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void AIABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AIABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AIABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("ChangeCameraView", IE_Pressed, this, &AIABaseCharacter::ChangeCameraView);

    PlayerInputComponent->BindAxis("CameraZoom", this, &AIABaseCharacter::CameraZoom);

    PlayerInputComponent->BindAxis("MoveForward", this, &AIABaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AIABaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &AIABaseCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AIABaseCharacter::LookUpRate);

    PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnAroundRate", this, &AIABaseCharacter::TurnAroundRate);
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
