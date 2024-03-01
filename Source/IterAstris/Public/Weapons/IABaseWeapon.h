// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IABaseWeapon.generated.h"

class UIAWeaponEnergyComponent;

UCLASS()
class ITERASTRIS_API AIABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AIABaseWeapon();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UIAWeaponEnergyComponent* WeaponEnergyComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* WeaponCrossHair;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName SocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.f;

    virtual void BeginPlay() override;

    virtual void MakeShot();

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    void MakeDamage(const FHitResult& HitResult);

public:
    virtual void StartFire();
    virtual void StopFire();
    virtual void HideWeapon();
    virtual void ShowWeapon();

    virtual UTexture2D* GetCrossHair() const { return WeaponCrossHair; };
    virtual UIAWeaponEnergyComponent* GetEnergyComponent() const { return WeaponEnergyComponent; };
};
