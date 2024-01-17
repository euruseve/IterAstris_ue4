// Iter Astris. All Rights Reserved.

#include "Components/IAHealthComponent.h"
#include "Player/IABaseCharacter.h"
#include "Gameframework/Actor.h"
#include <IACharacterMovementComponent.h>

UIAHealthComponent::UIAHealthComponent()
{
}


void UIAHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    Health = MaxHealth;

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UIAHealthComponent::OnTakeAnyDamage);
    }
}

void UIAHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
        Health -= Damage;
}

