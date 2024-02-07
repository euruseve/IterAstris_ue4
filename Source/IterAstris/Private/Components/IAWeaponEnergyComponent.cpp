// Iter Astris. All Rights Reserved.

#include "Components/IAWeaponEnergyComponent.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponEnergyComponent, All, All);

UIAWeaponEnergyComponent::UIAWeaponEnergyComponent() {}

void UIAWeaponEnergyComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentEnergyAmount = FullEnergyAmount;
    RechargingAmount = RechargeTime;
}

void UIAWeaponEnergyComponent::Recharge()
{
    if (!GetWorld())
        return;

    bIsRecharged = false;
    RechargingAmount = 0.0f;

    FTimerHandle IsRechargedTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        IsRechargedTimerHandle, this, &UIAWeaponEnergyComponent::SetRecharge, RechargeTime, false);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UIAWeaponEnergyComponent::UdpateRecharge, 0.1, true);
}

void UIAWeaponEnergyComponent::SetRecharge()
{
    bIsRecharged = true;
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("bIsRecharged %s"), (bIsRecharged ? TEXT("1") : TEXT("2")));
}

void UIAWeaponEnergyComponent::UdpateRecharge() 
{
    RechargingAmount += 0.1;
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("RechargingAmount %f"), RechargingAmount);
    if (RechargingAmount >= RechargeTime)
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


void UIAWeaponEnergyComponent::ReduceEnergy()
{
    CurrentEnergyAmount -= DecreaseEnergyAmount;
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("CurrentEnergyAmount %f"), CurrentEnergyAmount);
}
