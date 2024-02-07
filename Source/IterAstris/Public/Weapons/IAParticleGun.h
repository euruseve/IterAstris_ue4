// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/IABaseWeapon.h"
#include "IAParticleGun.generated.h"

UCLASS()
class ITERASTRIS_API AIAParticleGun : public AIABaseWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0"))
    float BulletSpread = 1.5f;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

public:
    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void HideWeapon() override;
    virtual void ShowWeapon() override;
    virtual UTexture2D* GetCrossHair() const override { return WeaponCrossHair; } ;

    virtual void BeginPlay() override;

private:
    FTimerHandle ShotTimerHandle;
    float TimeBetweenShots;
};
