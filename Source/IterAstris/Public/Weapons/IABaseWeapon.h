// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IABaseWeapon.generated.h"

UCLASS()
class ITERASTRIS_API AIABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AIABaseWeapon();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    virtual void BeginPlay() override;
};
