// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAIntoxicationZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneStateChanged, bool, InZone);

class UBoxComponent;

UCLASS()
class ITERASTRIS_API AIAIntoxicationZone : public AActor
{
    GENERATED_BODY()

public:
    AIAIntoxicationZone();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* ZoneCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* VisualMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Toxin")
    float OutsideDefaultToxinLevel = 0.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Toxin")
    float ToxinLevelOnEnter = 60.f;

    UPROPERTY(BlueprintAssignable, Category = "Toxin")
    FOnZoneStateChanged OnZoneStateChanged;

    virtual void BeginPlay() override;

public:
    UFUNCTION()
    FORCEINLINE float GetToxinLevel() const { return ToxinLevelOnEnter; };

private:
    UFUNCTION()
    void OnZoneEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnZoneExit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void IsInZone(AActor* Actor, bool InZone);
};
