// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAIntoxicationZone.generated.h"

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Toxin")
    float ToxinLevelOnEnter = 60.f; // Set your desired initial toxin level here

    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnZoneEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnZoneExit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ApplyToxinLevel(class AActor* Actor);
};
