// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITERASTRIS_API UIAHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIAHealthComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "100"))
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float Health = 0.f;

    virtual void BeginPlay() override;

public:
    FORCEINLINE float GetHealth() const { return Health; };

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
