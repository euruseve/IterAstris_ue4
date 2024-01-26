// Iter Astris. All Rights Reserved.

#include "Components/IAHealthComponent.h"
#include "Components/IAIntoxicationComponent.h"
#include "Player/IABaseCharacter.h"
#include "Gameframework/Actor.h"
#include <IACharacterMovementComponent.h>
#include "DamageTypes/IAIntoxicationDamageType.h"
    

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UIAHealthComponent::UIAHealthComponent() {}

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
    if (DamageType->IsA(UIAIntoxicationDamageType::StaticClass()))
    {
        UIAIntoxicationComponent* Intoxication = DamagedActor->FindComponentByClass<UIAIntoxicationComponent>();
        UE_LOG(LogHealthComponent, Warning, TEXT("This shit works"));
    }

}


