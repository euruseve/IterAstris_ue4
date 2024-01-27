// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/IAHealthComponent.h"
#include "IAPlayerHealthComponent.generated.h"

UCLASS()
class ITERASTRIS_API UIAPlayerHealthComponent : public UIAHealthComponent
{
    GENERATED_BODY()

public:
    UIAPlayerHealthComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0"))
    float HealthDecreaseDelay;

    virtual void BeginPlay() override;

private:
    float DamageToApply = 0.f;

    UFUNCTION()
    void OnToxinLevelChanged(float NewToxinLevel);

    UFUNCTION()
    void ApplyContinuousDamage();

    UFUNCTION()
    void StartContinuousDamage();

    UFUNCTION()
    void StopContinuousDamage();

    FTimerHandle ContinuousDamageTimerHandle;
};
