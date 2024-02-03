// Iter Astris. All Rights Reserved.

#include "Components/IAWeaponComponent.h"
#include "Weapons/IABaseWeapon.h"
#include "GameFramework/Character.h"

UIAWeaponComponent::UIAWeaponComponent() {}

void UIAWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    //SpawnWeapon("SpineWeaponSocket");
}

void UIAWeaponComponent::SpawnWeapon(FName SocetName)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    if (CurrentWeapon)
    {
        CurrentWeapon->AttachToComponent(
            Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), SocetName);
    }
    else
    {
        CurrentWeapon = GetWorld()->SpawnActor<AIABaseWeapon>(Weapon);
        if (CurrentWeapon)
        {
            CurrentWeapon->AttachToComponent(
                Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), SocetName);
        }
    }

    CurrentWeapon->SetOwner(Character);
}

void UIAWeaponComponent::Fire() 
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->Fire();
}

void UIAWeaponComponent::SetWeapon(FName SocketName)
{
    SpawnWeapon(SocketName);
}
