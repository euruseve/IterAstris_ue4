// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAIntoxicationComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITERASTRIS_API UIAIntoxicationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIAIntoxicationComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin", meta = (ClampMin = "0", ClampMax = "100"))
    float MaxToxinLvl = 100.f;

    virtual void BeginPlay() override;

public:
    FORCEINLINE float GetToxinLevel() const { return ToxinLvl; };

private:
    float ToxinLvl = 0.f;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
        class AController* InstigatedBy, AActor* DamageCauser);
};
