// Iter Astris. All Rights Reserved.

#include "UI/IAPlayerHUDWidget.h"
#include "Components/IAIntoxicationComponent.h"
#include "Components/IAHealthComponent.h"

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
