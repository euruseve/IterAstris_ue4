// Iter Astris. All Rights Reserved.

#include "UI/IAPlayerHUDWidget.h"
#include "Components/IAIntoxicationComponent.h"
#include "Components/IAHealthComponent.h"
#include "Components/IAWeaponComponent.h"
#include "Player/IABaseCharacter.h"
#include "IAUtils.h"

float UIAPlayerHUDWidget::GetToxinLevel() const
{
    const auto Intoxication = IAUtils::GetIAPlayerComponent<UIAIntoxicationComponent>(GetOwningPlayerPawn());
    if (!Intoxication)
        return 0.0f;

    return Intoxication->GetToxinLevel();
}

float UIAPlayerHUDWidget::GetHealth() const
{
    const auto Health = IAUtils::GetIAPlayerComponent<UIAHealthComponent>(GetOwningPlayerPawn());
    if (!Health)
        return 0.0f;

    return FMath::RoundToInt(Health->GetHealth());
}

float UIAPlayerHUDWidget::GetRechargePercent() const
{
    const auto Weapon = IAUtils::GetIAPlayerComponent<UIAWeaponComponent>(GetOwningPlayerPawn());
    if (!Weapon)
        return 0.f;

    return Weapon->GetRechargeTimePercent();
}

float UIAPlayerHUDWidget::GetEnergyAmount() const
{
    const auto Weapon = IAUtils::GetIAPlayerComponent<UIAWeaponComponent>(GetOwningPlayerPawn());
    if (!Weapon)
        return 0.f;

    return Weapon->GetEnergyAmount();
}

UTexture2D* UIAPlayerHUDWidget::GetCrossHairIcon() const
{
    const auto Weapon = IAUtils::GetIAPlayerComponent<UIAWeaponComponent>(GetOwningPlayerPawn());
    if (!Weapon)
        return nullptr;

    return Weapon->GetCrossHair();
}

bool UIAPlayerHUDWidget::IsWeaponOnPlayerEquiped() const
{
    const auto Player = Cast<AIABaseCharacter>(GetOwningPlayerPawn());

    return Player && Player->IsWeaponEquiped();
}