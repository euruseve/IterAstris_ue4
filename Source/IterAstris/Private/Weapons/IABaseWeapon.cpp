// Iter Astris. All Rights Reserved.

#include "Weapons/IABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

AIABaseWeapon::AIABaseWeapon()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(WeaponMesh);
}

void AIABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

}
