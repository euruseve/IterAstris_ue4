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
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = .5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

private:
    FTimerHandle ShotTimerHandle;
};
