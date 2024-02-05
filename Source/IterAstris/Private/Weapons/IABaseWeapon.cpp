// Iter Astris. All Rights Reserved.

#include "Weapons/IABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Gameframework/Character.h"
#include "Gameframework/Controller.h"
DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AIABaseWeapon::AIABaseWeapon()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(WeaponMesh);
}

void AIABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
}

void AIABaseWeapon::StartFire() {
}

void AIABaseWeapon::StopFire() {
}
void AIABaseWeapon::MakeShot() {
    
}


APlayerController* AIABaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

bool AIABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector AIABaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(SocketName);
}

bool AIABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void AIABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AIABaseWeapon::MakeDamage(const FHitResult& HitResult) 
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor)
        return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}