// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IACharacterMovementComponent.generated.h"

UCLASS()
class ITERASTRIS_API UIACharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "2", ClampMax = "5"))
    float RunModifier = 3.f;

    virtual float GetMaxSpeed() const override;
};
