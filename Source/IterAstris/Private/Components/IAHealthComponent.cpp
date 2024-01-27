// Iter Astris. All Rights Reserved.

#include "Components/IAHealthComponent.h"
#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UIAHealthComponent::UIAHealthComponent() {}

void UIAHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    UIAIntoxicationComponent* Intoxication = GetOwner()->FindComponentByClass<UIAIntoxicationComponent>();
    if (Intoxication)
    {
        Intoxication->OnToxinLevelChanged.AddDynamic(this, &UIAHealthComponent::OnToxinLevelChanged);
    }
}

void UIAHealthComponent::OnToxinLevelChanged(float NewToxinLevel)
{
    DamageToApply = NewToxinLevel;
    if (NewToxinLevel > 0.f)
    {
        StartContinuousDamage();
    }
    else
    {
        StopContinuousDamage();
    }
}

void UIAHealthComponent::ApplyContinuousDamage()
{
    Health -= DamageToApply * 0.1f;
}

void UIAHealthComponent::StartContinuousDamage()
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(ContinuousDamageTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(
            ContinuousDamageTimerHandle, this, &UIAHealthComponent::ApplyContinuousDamage, 1.0f, true, 5.0f);
    }
}

void UIAHealthComponent::StopContinuousDamage()
{
    GetWorld()->GetTimerManager().ClearTimer(ContinuousDamageTimerHandle);
}