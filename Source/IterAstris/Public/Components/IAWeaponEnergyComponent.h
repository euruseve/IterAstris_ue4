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
    UPROPERTY(EditDefaultsOnly, Category = "Energy", meta = (ClampMin = "0"))
    float FullEnergyAmount = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Energy", meta = (ClampMin = "0"))
    float DecreaseEnergyAmount = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "Energy", meta = (ClampMin = "0"))
    float RechargeTime = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "Energy", meta = (ClampMin = "0"))
    float RateOfRecharging = 0.01f;

    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetRechargeTimePercent() const { return RechargingAmount / RechargeTime; };

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetEnergyAmount() const { return CurrentEnergyAmount / FullEnergyAmount; };

    float GetRechargeTime() const { return RechargeTime; };
    bool IsRecharged() const { return bIsRecharged; };

    void ReduceEnergy();
    void Recharge();

private:
    FTimerHandle TimerHandle;

    bool bIsRecharged = true;
    float RechargingAmount = 0.f;

    float CurrentEnergyAmount = 0.f;

    void UdpateRecharge();
};
