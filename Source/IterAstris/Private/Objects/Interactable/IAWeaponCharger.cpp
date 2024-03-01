// Iter Astris. All Rights Reserved.

#include "Objects/Interactable/IAWeaponCharger.h"
#include "Components/IAWeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Player/IABaseCharacter.h>

AIAWeaponCharger::AIAWeaponCharger()
{
    _Root = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    RootComponent = _Root;

    ChargerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ChargerSkeletalMesh");
    ChargerMesh->SetupAttachment(GetRootComponent());

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMesh");
    WeaponMesh->SetupAttachment(ChargerMesh, "GunPlace");

    InteractionBox = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    InteractionBox->SetupAttachment(GetRootComponent());

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(GetRootComponent());

    ChargingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ChargingWidget"));
    ChargingWidget->SetupAttachment(GetRootComponent());
}

void AIAWeaponCharger::BeginPlay()
{
    Super::BeginPlay();
    InteractionWidget->SetVisibility(false);
    ChargingWidget->SetVisibility(false);
}

void AIAWeaponCharger::Charge()
{
    WeaponMesh->SetVisibility(bIsCharging);
    ChargingWidget->SetVisibility(bIsCharging);

    const auto Player = Cast<AIABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    Player->SetWeaponCharging(bIsCharging);

    if (bIsCharging)
        GetWorld()->GetTimerManager().SetTimer(
            ChargingTimerHandler, this, &AIAWeaponCharger::SetEnergyAmount, ChargingTime, true);
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandler);
        Player->GetWeaponComponent()->SetEnergy(FMath::Floor(Player->GetWeaponComponent()->GetCurrentEnergy()));
    }
}

void AIAWeaponCharger::SetEnergyAmount()
{
    const auto Player = Cast<AIABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    const auto WeaponEnergy = Player->GetWeaponComponent()->GetCurrentEnergy();
    Player->GetWeaponComponent()->SetEnergy(WeaponEnergy + ChargingAmount);

    UE_LOG(LogTemp, Display, TEXT("Energy is %f now"), WeaponEnergy);

    if (Player->GetWeaponComponent()->GetCurrentEnergy() >= Player->GetWeaponComponent()->GetFullEnergy())
    {
        GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandler);
    }
}

void AIAWeaponCharger::Interact_Implementation()
{
    bIsCharging = !bIsCharging;

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AIAWeaponCharger::Charge, .5f, false);
}

void AIAWeaponCharger::ShowInteractionWidget_Implementation()
{
    InteractionWidget->SetVisibility(true);
}

void AIAWeaponCharger::HideInteractionWidget_Implementation()
{
    InteractionWidget->SetVisibility(false);
}
