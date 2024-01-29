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
    UE_LOG(LogIntoxicationComponent, Display, TEXT("SetToxinLevel NewToxinLvl: %f --- InCurrentZoneToxinLvl: %f"),
        NewToxinLvl, InCurrentZoneToxinLvl);
}

void UIAIntoxicationComponent::HandleZoneStateChanged(bool InZone)
{
    bIsInIntoxicationZone = InZone;
}