// Iter Astris. All Rights Reserved.

#include "Components/IAHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UIAHealthComponent::UIAHealthComponent() {}

void UIAHealthComponent::BeginPlay()
{
    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);
    Super::BeginPlay();
}
