// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAIntoxicationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToxinLevelChanged, float, NewToxinLevel);

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
    float OwnToxinLvl = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float DecreasingValue = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float DecreasingRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Toxin")
    float DecreasingDelay = 1.f;

    virtual void UpdateToxinLevel(){};

    virtual void DecreaseToxinLevel(){};

    float InCurrentZoneToxinLvl = 0.f;
    bool bIsInIntoxicationZone = false;

    AActor* OwnerActor;
    FTimerHandle ToxinUpdateTimerHandle;

    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintAssignable, Category = "Toxin")
    FOnToxinLevelChanged OnToxinLevelChanged;

    FORCEINLINE bool IsInIntoxicationZone() const { return bIsInIntoxicationZone; };

    UFUNCTION(BlueprintCallable, Category = "Toxin")
    float GetToxinLevel() const { return OwnToxinLvl; };
   
    virtual void SetCurrentZoneToxinLevel(float NewToxinLvl);

    virtual void HandleZoneStateChanged(bool InZone);
};
