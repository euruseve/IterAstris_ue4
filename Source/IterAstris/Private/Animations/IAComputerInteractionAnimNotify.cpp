// Iter Astris. All Rights Reserved.

#include "Animations/IAComputerInteractionAnimNotify.h"

void UIAComputerInteractionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnComputerInteractionNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
