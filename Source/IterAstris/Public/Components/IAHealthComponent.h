// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITERASTRIS_API UIAHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIAHealthComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0"))
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float Health = 0.f;

    UPROPERTY()
    bool bCanToBroadcast = true;

    virtual void BeginPlay() override;

public:
    float GetHealth() const { return Health; };

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); };

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
};
