// Iter Astris. All Rights Reserved.

#include "Weapons/IAParticleGun.h"
#include "DrawDebugHelpers.h"
#include "Components/IAWeaponEnergyComponent.h"

void AIAParticleGun::StartFire()
{
    Super::StartFire();

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

    if (WeaponEnergyComponent->GetEnergyAmount() <= 0.f)
        return;

    WeaponEnergyComponent->ReduceEnergy();
    WeaponEnergyComponent->Recharge();

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

void AIAParticleGun::HideWeapon()
{
    // +2 sec needed cuz of "void AIABaseCharacter::UnequipWeapon()"

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponMesh->SetOwnerNoSee(true); }, 3.5f, false);
}

void AIAParticleGun::ShowWeapon()
{
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, [this]() { WeaponMesh->SetOwnerNoSee(false); }, 1.0f, false);
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