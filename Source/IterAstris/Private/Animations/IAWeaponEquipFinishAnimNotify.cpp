// Iter Astris. All Rights Reserved.


#include "Animations/IAWeaponEquipFinishAnimNotify.h"

void UIAWeaponEquipFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnWeaponNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
