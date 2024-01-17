// Iter Astris. All Rights Reserved.

#include "Components/IAIntoxicationComponent.h"
#include "Gameframework/Actor.h"

UIAIntoxicationComponent::UIAIntoxicationComponent() {}

void UIAIntoxicationComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UIAIntoxicationComponent::OnTakeAnyDamage);
    }
}

void UIAIntoxicationComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    ToxinLvl += Damage;
}