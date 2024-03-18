// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "IAComputerInteractionAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class ITERASTRIS_API UIAComputerInteractionAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    FOnNotifiedSignature OnComputerInteractionNotified;
};
