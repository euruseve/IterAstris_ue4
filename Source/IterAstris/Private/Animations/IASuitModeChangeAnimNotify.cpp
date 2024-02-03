// Iter Astris. All Rights Reserved.


#include "Animations/IASuitModeChangeAnimNotify.h"

void UIASuitModeChangeAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
    OnSuitNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
