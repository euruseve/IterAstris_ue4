// Iter Astris. All Rights Reserved.

#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"

UIAIntoxicationComponent::UIAIntoxicationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UIAIntoxicationComponent::BeginPlay()
{
    Super::BeginPlay();
    ToxinLvl = 0.f;
    OwnerActor = GetOwner();
}

void UIAIntoxicationComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateToxinLevel(DeltaTime);
}

void UIAIntoxicationComponent::UpdateToxinLevel(float DeltaTime)
{
    if (ToxinLvl > 0.f)
    {
        if (ToxinDecreaseTimer <= 0.f)
        {
            ToxinLvl -= ToxinDecreaseRate * DeltaTime;
            ToxinLvl = FMath::Clamp(ToxinLvl, 0.f, MaxToxinLvl);
        }
        else
        {
            ToxinDecreaseTimer -= DeltaTime;
        }
    }
}

void UIAIntoxicationComponent::SetToxinLevel(float NewToxinLevel)
{
    ToxinLvl = FMath::Clamp(NewToxinLevel, 0.f, MaxToxinLvl);
    ToxinDecreaseTimer = ToxinDecreaseDelay;
}
