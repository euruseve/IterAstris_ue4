// Iter Astris. All Rights Reserved.

#include "Components/IAWeaponEnergyComponent.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponEnergyComponent, All, All);

UIAWeaponEnergyComponent::UIAWeaponEnergyComponent() {}

void UIAWeaponEnergyComponent::BeginPlay()
{
    Super::BeginPlay();
    check(FullEnergyAmount > 0);

    CurrentEnergyAmount = FullEnergyAmount;
    RechargingAmount = RechargeTime;
}

void UIAWeaponEnergyComponent::Recharge()
{
    if (!GetWorld())
        return;

    bIsRecharged = false;
    RechargingAmount = 0.0f;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &UIAWeaponEnergyComponent::UdpateRecharge, RateOfRecharging, true);
}

void UIAWeaponEnergyComponent::UdpateRecharge()
{
    RechargingAmount += RateOfRecharging;
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("RechargingAmount %f"), RechargingAmount);
    if (RechargingAmount >= RechargeTime)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        bIsRecharged = true;

        UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("bIsRecharged %s"), (bIsRecharged ? TEXT("1") : TEXT("0")));
    }
}

void UIAWeaponEnergyComponent::ReduceEnergy()
{
    CurrentEnergyAmount -= DecreaseEnergyAmount;

    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("CurrentEnergyAmount %f"), CurrentEnergyAmount);
}