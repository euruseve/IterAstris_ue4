// Iter Astris. All Rights Reserved.

#include "UI/IAPlayerHUDWidget.h"
#include "Components/IAIntoxicationComponent.h"
#include "Components/IAHealthComponent.h"
#include "Components/IAWeaponComponent.h"
#include "Player/IABaseCharacter.h"

float UIAPlayerHUDWidget::GetToxinLevel() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    const auto Component = Player->GetComponentByClass(UIAIntoxicationComponent::StaticClass());
    const auto Intoxication = Cast<UIAIntoxicationComponent>(Component);
    if (!Intoxication)
        return 0.0f;

    return Intoxication->GetToxinLevel();
}

float UIAPlayerHUDWidget::GetHealth() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    const auto Component = Player->GetComponentByClass(UIAHealthComponent::StaticClass());
    const auto Health = Cast<UIAHealthComponent>(Component);
    if (!Health)
        return 0.0f;

    return FMath::RoundToInt(Health->GetHealth());
}

UTexture2D* UIAPlayerHUDWidget::GetCrossHairIcon() const
{
    const auto Pawn = GetOwningPlayerPawn();
    if (!Pawn)
        return nullptr;

    const auto Component = Pawn->GetComponentByClass(UIAWeaponComponent::StaticClass());
    const auto Weapon = Cast<UIAWeaponComponent>(Component);
    if (!Weapon)
        return nullptr;

    return Weapon->GetCrossHair();
}

bool UIAPlayerHUDWidget::IsWeaponOnPlayerEquiped() const
{
    const auto Pawn = GetOwningPlayerPawn();
    if (!Pawn)
        return false;

    const auto Player = Cast<AIABaseCharacter>(Pawn);

    return Player && Player->IsWeaponEquiped();
}