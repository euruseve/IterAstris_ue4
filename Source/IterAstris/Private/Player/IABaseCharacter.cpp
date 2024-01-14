// Iter Astris. All Rights Reserved.

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

    BaseTurnRate = 45.f;
    CameraView = ECameraView::ThirdPersonView;

    // TODO взнати що воно робить
    // GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    // GetCharacterMovement()->JumpZVelocity = 600.f;
    // GetCharacterMovement()->AirControl = 0.2f;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

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

    PlayerInputComponent->BindAction("ChangeCameraView", IE_Pressed, this, &AIABaseCharacter::SwitchCameraView);

    PlayerInputComponent->BindAxis("MoveForward", this, &AIABaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AIABaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &AIABaseCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AIABaseCharacter::LookUpRate);

    PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnAroundRate", this, &AIABaseCharacter::TurnAroundRate);
}

void AIABaseCharacter::MoveForward(float Amount)
{
    if (Controller && (Amount != 0.f))
    {
        if (CameraView == ECameraView::ThirdPersonView)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

            AddMovementInput(Direction, Amount);
        }
        else if (CameraView == ECameraView::FirstPersonView)
        {
            AddMovementInput(GetActorForwardVector(), Amount);
        }
    }
}

void AIABaseCharacter::MoveRight(float Amount)
{
    if (Controller && (Amount != 0.f))
    {
        if (CameraView == ECameraView::ThirdPersonView)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

            AddMovementInput(Direction, Amount);
        }
        else if (CameraView == ECameraView::FirstPersonView)
        {
            AddMovementInput(GetActorRightVector(), Amount);
        }
    }
}

void AIABaseCharacter::LookUp(float Amount)
{
    SetCameraViewSettings(CameraView);

    AddControllerPitchInput(Amount);
}

void AIABaseCharacter::TurnAround(float Amount)
{
    SetCameraViewSettings(CameraView);

    AddControllerYawInput(Amount);
}

void AIABaseCharacter::TurnAroundRate(float Rate)
{
    if (CameraView == ECameraView::ThirdPersonView)
        AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIABaseCharacter::LookUpRate(float Rate)
{
    if (CameraView == ECameraView::ThirdPersonView)
        AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIABaseCharacter::SwitchCameraView()
{
    switch (CameraView)
    {
    case ECameraView::FirstPersonView:
        CameraView = ECameraView::ThirdPersonView;
        SpringArmComponent->TargetArmLength = 300;
        UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to TP"));
        break;

    case ECameraView::ThirdPersonView:
        CameraView = ECameraView::FirstPersonView;
        SpringArmComponent->TargetArmLength = 0;
        UE_LOG(LogBaseCharacter, Display, TEXT("CameraView have changed to FP"));
        break;

    default:
        UE_LOG(LogBaseCharacter, Error, TEXT("CameraView have not choosen!"));
        break;
    }
}

void AIABaseCharacter::SetCameraViewSettings(ECameraView Camera)
{
    if (Camera == ECameraView::ThirdPersonView)
    {
        bUseControllerRotationYaw = false;
        GetCharacterMovement()->bOrientRotationToMovement = true;
    }
    else if (Camera == ECameraView::FirstPersonView)
    {
        bUseControllerRotationYaw = true;
        GetCharacterMovement()->bOrientRotationToMovement = false;
    }
}
