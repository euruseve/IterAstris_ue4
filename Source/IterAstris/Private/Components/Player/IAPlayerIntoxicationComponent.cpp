// Iter Astris. All Rights Reserved.

#include "Components/Player/IAPlayerIntoxicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerIntoxicationComponent, All, All);

UIAPlayerIntoxicationComponent::UIAPlayerIntoxicationComponent() {}

void UIAPlayerIntoxicationComponent::BeginPlay()
{
    OwnToxinLvl = 0.f;
    OwnerActor = GetOwner();

    Super::BeginPlay();
}

void UIAPlayerIntoxicationComponent::SetCurrentZoneToxinLevel(float NewToxinLvl)
{
    Super::SetCurrentZoneToxinLevel(NewToxinLvl);
}

void UIAPlayerIntoxicationComponent::HandleZoneStateChanged(bool InZone)
{
    Super::HandleZoneStateChanged(InZone);

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

    UE_LOG(LogPlayerIntoxicationComponent, Warning,
        TEXT("HandleZoneStateChanged OwnToxinLvl: %f --- InCurrentZoneToxinLvl: %f"), OwnToxinLvl,
        InCurrentZoneToxinLvl);
    OnToxinLevelChanged.Broadcast(OwnToxinLvl);
}

void UIAPlayerIntoxicationComponent::UpdateToxinLevel()
{
    GetWorld()->GetTimerManager().ClearTimer(ToxinUpdateTimerHandle);

    if (!bIsInIntoxicationZone || OwnToxinLvl != InCurrentZoneToxinLvl)
    {
        GetWorld()->GetTimerManager().SetTimer(ToxinUpdateTimerHandle, this,
            &UIAPlayerIntoxicationComponent::DecreaseToxinLevel, DecreasingRate, true, DecreasingDelay);
    }
}

void UIAPlayerIntoxicationComponent::DecreaseToxinLevel()
{
    UE_LOG(LogPlayerIntoxicationComponent, Warning, TEXT("OwnToxinLvl: %f --- InCurrentZoneToxinLvl: %f"), OwnToxinLvl,
        InCurrentZoneToxinLvl);

    OwnToxinLvl = FMath::Max(OwnToxinLvl - DecreasingValue, InCurrentZoneToxinLvl);

    OnToxinLevelChanged.Broadcast(OwnToxinLvl);

    if (OwnToxinLvl == InCurrentZoneToxinLvl)
    {
        GetWorld()->GetTimerManager().ClearTimer(ToxinUpdateTimerHandle);
    }
}