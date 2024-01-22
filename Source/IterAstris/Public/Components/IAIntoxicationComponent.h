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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float ToxinDecreaseRate = 1.f; // Decrease rate per second

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float MaxToxinDamageRate = 1.f; // Maximum damage rate at MaxToxinLvl

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float ToxinDecreaseDelay = 5.f; // Delay before toxin starts decreasing after leaving the zone

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    FORCEINLINE float GetToxinLevel() const { return ToxinLvl; };
    void SetToxinLevel(float NewToxinLevel); // Added function to set toxin level externally

private:
    float ToxinLvl = 0.f;
    float ToxinDecreaseTimer = 0.f;
    AActor* OwnerActor;

    void UpdateToxinLevel(float DeltaTime);
};
