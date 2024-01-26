// Iter Astris. All Rights Reserved.

#include "DamageZones/IAIntoxicationZone.h"
#include "Components/BoxComponent.h"
#include <Components/IAIntoxicationComponent.h>
#include <DamageTypes/IAIntoxicationDamageType.h>

AIAIntoxicationZone::AIAIntoxicationZone()
{
    PrimaryActorTick.bCanEverTick = false;

    ZoneCollision = CreateDefaultSubobject<UBoxComponent>("ZoneCollision");
    RootComponent = ZoneCollision;
    ZoneCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ZoneCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZoneCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualMesh");
    VisualMesh->SetupAttachment(RootComponent);

    ToxinLevelOnEnter = 60.f;
}

void AIAIntoxicationZone::BeginPlay()
{
    Super::BeginPlay();

    ZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &AIAIntoxicationZone::OnZoneEnter);
    ZoneCollision->OnComponentEndOverlap.AddDynamic(this, &AIAIntoxicationZone::OnZoneExit);
}

void AIAIntoxicationZone::OnZoneEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IsInZone(OtherActor, true);
}

void AIAIntoxicationZone::OnZoneExit(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    IsInZone(OtherActor, false);
}

void AIAIntoxicationZone::IsInZone(AActor* Actor, bool InZone = false)
{
    UIAIntoxicationComponent* IntoxicationComponent = Actor->FindComponentByClass<UIAIntoxicationComponent>();
    if (IntoxicationComponent)
    {
        IntoxicationComponent->SetCurrentZoneToxinLevel(InZone ? ToxinLevelOnEnter : OutsideDefaultToxinLevel);
        IntoxicationComponent->HandleZoneStateChanged(InZone);
    }
    OnZoneStateChanged.Broadcast(InZone);
}
