// Iter Astris. All Rights Reserved.


#include "Components/IACharacterMovementComponent.h"
#include "Player/IABaseCharacter.h"

float UIACharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AIABaseCharacter* Player = Cast<AIABaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
