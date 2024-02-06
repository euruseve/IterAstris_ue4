// Iter Astris. All Rights Reserved.

#include "Components/IAWeaponComponent.h"
#include "Weapons/IABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Components/IAWeaponEnergyComponent.h"

UIAWeaponComponent::UIAWeaponComponent() {}

void UIAWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon("SpineWeaponSocket");
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

void UIAWeaponComponent::StartFire() 
{
    if (!CurrentWeapon )
        return;

    if (CurrentWeapon->GetEnergyComponent()->GetEnergyAmount() > 0.f)
        CurrentWeapon->StartFire();
}

void UIAWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->StopFire();
}

void UIAWeaponComponent::SetWeapon(FName SocketName)
{
    SpawnWeapon(SocketName);
}

UTexture2D* UIAWeaponComponent::GetCrossHair() const
{
    if (!CurrentWeapon)
        return nullptr;

    return CurrentWeapon->GetCrossHair();
}

void UIAWeaponComponent::HideWeapon() 
{
    CurrentWeapon->HideWeapon();
}

void UIAWeaponComponent::ShowWeapon() 
{
    CurrentWeapon->ShowWeapon();
}
