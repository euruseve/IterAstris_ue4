// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAWeaponEnergyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITERASTRIS_API UIAWeaponEnergyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIAWeaponEnergyComponent();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Energy")
    float EnergyAmount = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Energy")
    float DecreaseEnergyAmount = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "Energy", meta = (ClampMin = "1"))
    float RechargeTime = 1.f;

    virtual void BeginPlay() override;

public:
    float GetEnergyAmount() const { return EnergyAmount; };
    float GetRechargeTime() const { return RechargeTime; };

    void ReduceEnergy();
    void Recharge();
};
