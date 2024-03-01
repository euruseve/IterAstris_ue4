// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAWeaponComponent.generated.h"

class AIABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITERASTRIS_API UIAWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIAWeaponComponent();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AIABaseWeapon> Weapon;

    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Energy")
    float GetRechargeTimePercent() const;

    UFUNCTION(BlueprintCallable, Category = "Energy")
    float GetEnergyAmount() const;

    UFUNCTION(BlueprintCallable, Category = "Energy")
    float GetFullEnergy() const;

    UFUNCTION(BlueprintCallable, Category = "Energy")
    float GetCurrentEnergy() const;

    UFUNCTION(BlueprintCallable, Category = "Energy")
    void SetEnergy(float EnergyCount);

    void StartFire();
    void StopFire();
    void SetWeapon(FName SocketName);
    void HideWeapon();
    void ShowWeapon();

    UTexture2D* GetCrossHair() const;

private:
    UPROPERTY()
    AIABaseWeapon* CurrentWeapon = nullptr;

    void SpawnWeapon(FName SocetName);
};
