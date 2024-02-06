// Iter Astris. All Rights Reserved.

#include "Components/IAWeaponEnergyComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponEnergyComponent, All, All);

UIAWeaponEnergyComponent::UIAWeaponEnergyComponent() {}
void UIAWeaponEnergyComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UIAWeaponEnergyComponent::Recharge() 
{
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("Recharge"));
}

void UIAWeaponEnergyComponent::ReduceEnergy()
{
    EnergyAmount -= DecreaseEnergyAmount;
    UE_LOG(LogWeaponEnergyComponent, Warning, TEXT("EnergyAmount %f"), EnergyAmount);
}
