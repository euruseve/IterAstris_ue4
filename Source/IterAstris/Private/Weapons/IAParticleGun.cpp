// Iter Astris. All Rights Reserved.

#include "Weapons/IAParticleGun.h"
#include "DrawDebugHelpers.h"

void AIAParticleGun::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AIAParticleGun::MakeShot, TimeBetweenShots, true);
}

void AIAParticleGun::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}
void AIAParticleGun::MakeShot()
{
    if (!GetWorld())
        return;

    FVector TraceStart, TraceEnd;

    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Yellow, false, 3.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24.f, FColor::Red, false, 5.f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Yellow, false, 3.f, 0, 3.f);
    }
}

bool AIAParticleGun::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}