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
    float HealthDecreaseDelay = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Heal")
    bool bAutoHeal = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Heal")
    float HealUpdateTime = 1.f;    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Heal")
    float HealthHealDelay = 1.f;  
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Heal")
    float HealModifier = 1.f;


    virtual void BeginPlay() override;
private:
    AActor* OwnerActor;
    float DamageToApply = 0.f;

    UFUNCTION()
    void OnToxinLevelChanged(float NewToxinLevel);

    UFUNCTION()
    void ApplyContinuousDamage();

    UFUNCTION()
    void StartContinuousDamage();

    UFUNCTION()
    void StopContinuousDamage();

    void HealUpdate();
    void StopHeal();
    void SetHealth(float Value);

    FTimerHandle ContinuousDamageTimerHandle;
    FTimerHandle HealTimerHandle;
};
