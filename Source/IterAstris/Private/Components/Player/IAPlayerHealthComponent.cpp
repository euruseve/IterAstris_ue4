// Iter Astris. All Rights Reserved.

#include "Components/Player/IAPlayerHealthComponent.h"
#include <Player/IABaseCharacter.h>
#include "Components/Player/IAPlayerIntoxicationComponent.h"
#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerHealthComponent, All, All);

UIAPlayerHealthComponent::UIAPlayerHealthComponent() {}

void UIAPlayerHealthComponent::BeginPlay()
{
    HealthDecreaseDelay = 1.f;
    OwnerActor = GetOwner();

    check(OwnerActor);

    UIAPlayerIntoxicationComponent* Intoxication = OwnerActor->FindComponentByClass<UIAPlayerIntoxicationComponent>();
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
    AIABaseCharacter* Character = Cast<AIABaseCharacter>(OwnerActor);

    if (!Character)
        return;

    // These values are random tbh
    if (Character->IsPlayerInCostume() && DamageToApply >= 80)
        Health -= DamageToApply * 0.01f;
    else if (!Character->IsPlayerInCostume())
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