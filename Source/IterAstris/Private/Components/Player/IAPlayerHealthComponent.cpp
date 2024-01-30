// Iter Astris. All Rights Reserved.

#include "Components/Player/IAPlayerHealthComponent.h"
#include <Player/IABaseCharacter.h>
#include "Components/Player/IAPlayerIntoxicationComponent.h"
#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerHealthComponent, All, All);

UIAPlayerHealthComponent::UIAPlayerHealthComponent() {}

void UIAPlayerHealthComponent::BeginPlay()
{
    OwnerActor = GetOwner();

    check(OwnerActor);

    if (OwnerActor)
    {
        OwnerActor->OnTakeAnyDamage.AddDynamic(this, &UIAPlayerHealthComponent::OnTakeAnyDamageHandle);
    }

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
    StopHeal();
    AIABaseCharacter* Character = Cast<AIABaseCharacter>(OwnerActor);

    if (!Character && GetWorld())
        return;

    if (Character->IsPlayerInCostume() && DamageToApply >= 80)
    {
        SetHealth(Health - DamageToApply * 0.01f);
    }
    else if (!Character->IsPlayerInCostume())
    {
        SetHealth(Health - DamageToApply * 0.1f);
    }

    if (IsDead() && bCanToBroadcast)
    {
        bCanToBroadcast = false;
        OnDeath.Broadcast();
    }
    else if (bAutoHeal)
    {
        StartHeal();
    }
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

void UIAPlayerHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        StopHeal();
    }
}

void UIAPlayerHealthComponent::StartHeal()
{
    GetWorld()->GetTimerManager().SetTimer(
        HealTimerHandle, this, &UIAPlayerHealthComponent::HealUpdate, HealUpdateTime, true, HealthHealDelay);
}

void UIAPlayerHealthComponent::StopHeal()
{
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
}

void UIAPlayerHealthComponent::SetHealth(float Value)
{
    Health = FMath::Clamp(Value, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

void UIAPlayerHealthComponent::OnTakeAnyDamageHandle(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.f || IsDead() || !GetWorld())
        return;

    StopHeal();

    if (IsDead() || FMath::IsNearlyZero(Health))
    {
        OnDeath.Broadcast();
    }
    else if (bAutoHeal)
    {
        StartHeal();
    }

    // UE_LOG(LogPlayerHealthComponent, Display, TEXT("TYT %f"), Damage);
    SetHealth(Health - Damage);
}
