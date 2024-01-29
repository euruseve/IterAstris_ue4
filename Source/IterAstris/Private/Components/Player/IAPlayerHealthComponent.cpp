// Iter Astris. All Rights Reserved.

#include "Components/Player/IAPlayerHealthComponent.h"
#include "Components/Player/IAPlayerIntoxicationComponent.h"
#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerHealthComponent, All, All);

UIAPlayerHealthComponent::UIAPlayerHealthComponent() {}

void UIAPlayerHealthComponent::BeginPlay() 
{
    HealthDecreaseDelay = 1.f;
    UIAPlayerIntoxicationComponent* Intoxication = GetOwner()->FindComponentByClass<UIAPlayerIntoxicationComponent>();
    if (Intoxication)
    {
        Intoxication->OnToxinLevelChanged.AddDynamic(this, &UIAPlayerHealthComponent::OnToxinLevelChanged);
    }

    Super::BeginPlay();
}

void UIAPlayerHealthComponent::OnToxinLevelChanged(float NewToxinLevel)
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

void UIAPlayerHealthComponent::ApplyContinuousDamage()
{
    Health -= DamageToApply * 0.1f;
}

void UIAPlayerHealthComponent::StartContinuousDamage()
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(ContinuousDamageTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(ContinuousDamageTimerHandle, this,
            &UIAPlayerHealthComponent::ApplyContinuousDamage, 1.0f, true, HealthDecreaseDelay);
    }
}

void UIAPlayerHealthComponent::StopContinuousDamage()
{
    GetWorld()->GetTimerManager().ClearTimer(ContinuousDamageTimerHandle);
}