#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"
#include <DamageZones/IAIntoxicationZone.h>
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogIntoxicationComponent, All, All);

UIAIntoxicationComponent::UIAIntoxicationComponent() {}

void UIAIntoxicationComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnToxinLvl = 0.f;
    OwnerActor = GetOwner();
}

void UIAIntoxicationComponent::SetCurrentZoneToxinLevel(float NewToxinLvl)
{
    InCurrentZoneToxinLvl = NewToxinLvl;
    UE_LOG(LogIntoxicationComponent, Warning, TEXT("SetToxinLevel NewToxinLvl: %f --- InCurrentZoneToxinLvl: %f"),
        NewToxinLvl, InCurrentZoneToxinLvl);
}

void UIAIntoxicationComponent::HandleZoneStateChanged(bool InZone)
{
    bIsInIntoxicationZone = InZone;

    if (bIsInIntoxicationZone)
    {
        OwnToxinLvl = FMath::Max(InCurrentZoneToxinLvl, OwnToxinLvl);
    }
    else
    {
        if (OwnToxinLvl > InCurrentZoneToxinLvl)
        {
            UpdateToxinLevel();
        }
        else
        {
            OwnToxinLvl = InCurrentZoneToxinLvl;
        }
    }

    UE_LOG(LogIntoxicationComponent, Warning,
        TEXT("HandleZoneStateChanged OwnToxinLvl: %f --- InCurrentZoneToxinLvl: %f"), OwnToxinLvl,
        InCurrentZoneToxinLvl);
}

void UIAIntoxicationComponent::UpdateToxinLevel()
{
    GetWorld()->GetTimerManager().ClearTimer(ToxinUpdateTimerHandle);

    if (!bIsInIntoxicationZone || OwnToxinLvl != InCurrentZoneToxinLvl)
    {
        GetWorld()->GetTimerManager().SetTimer(ToxinUpdateTimerHandle, this,
            &UIAIntoxicationComponent::DecreaseToxinLevel, DecreasingRate, true, DecreasingDelay);
    }
}

void UIAIntoxicationComponent::DecreaseToxinLevel()
{
    UE_LOG(LogIntoxicationComponent, Warning, TEXT("OwnToxinLvl: %f --- InCurrentZoneToxinLvl: %f"), OwnToxinLvl,
        InCurrentZoneToxinLvl);

    OwnToxinLvl = FMath::Max(OwnToxinLvl - DecreasingValue, InCurrentZoneToxinLvl);

    OnToxinLevelChanged.Broadcast(OwnToxinLvl);

    if (OwnToxinLvl == InCurrentZoneToxinLvl)
    {
        GetWorld()->GetTimerManager().ClearTimer(ToxinUpdateTimerHandle);
    }
}