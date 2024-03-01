// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "IAWeaponCharger.generated.h"

UCLASS()
class ITERASTRIS_API AIAWeaponCharger : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AIAWeaponCharger();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USceneComponent* _Root;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USkeletalMeshComponent* ChargerMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UBoxComponent* InteractionBox;

    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    class UWidgetComponent* InteractionWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    class UWidgetComponent* ChargingWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Charging", meta = (ClampMin = "0.01"))
    float ChargingTime = 1.f;    
    
    UPROPERTY(EditDefaultsOnly, Category = "Charging", meta = (ClampMin = "0.01"))
    float ChargingAmount = 1.f;

public:
    virtual void Interact_Implementation() override;
    virtual void ShowInteractionWidget_Implementation() override;
    virtual void HideInteractionWidget_Implementation() override;

private:
    FTimerHandle ChargingTimerHandler;

    bool bIsCharging = false;

    void Charge();
    void SetEnergyAmount();
};
