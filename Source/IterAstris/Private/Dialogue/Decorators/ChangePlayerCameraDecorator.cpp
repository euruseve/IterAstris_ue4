// Iter Astris. All Rights Reserved.


#include "Dialogue/Decorators/ChangePlayerCameraDecorator.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>

void UChangePlayerCameraDecorator::InitializeDecorator_Implementation(
    UWorld* World, const TScriptInterface<IMounteaDialogueParticipantInterface>& OwningParticipant)
{
    Super::InitializeDecorator_Implementation(World, OwningParticipant);

    PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);

    const auto Camera1 = PlayerPawn->GetComponentsByTag(UCameraComponent::StaticClass(), FName("DefaultCamera"));
    const auto Camera2 = PlayerPawn->GetComponentsByTag(UCameraComponent::StaticClass(), FName("DialogueCamera"));
    const auto Camera3 = PlayerPawn->GetComponentsByTag(UCameraComponent::StaticClass(), FName("FirstLookCamera"));

    PlayerDefaultCamera = Cast<UCameraComponent>(Camera1[0]);
    PlayerDialogueCamera = Cast<UCameraComponent>(Camera2[0]);
    PlayerFirstLookCamera = Cast<UCameraComponent>(Camera3[0]);
}

bool UChangePlayerCameraDecorator::ValidateDecorator_Implementation(TArray<FText>& ValidationMessages)
{
    if ((PlayerDefaultCamera && PlayerDialogueCamera && PlayerFirstLookCamera) && GetWorld())
        ChangeCamera(true);

    return Super::ValidateDecorator_Implementation(ValidationMessages);
}

void UChangePlayerCameraDecorator::CleanupDecorator_Implementation() 
{
    if ((PlayerDefaultCamera && PlayerDialogueCamera && PlayerFirstLookCamera))
        ChangeCamera(false);
 
    Super::CleanupDecorator_Implementation();
}

void UChangePlayerCameraDecorator::ChangeCamera(bool IsDialogStarted) 
{
    if ((PlayerDefaultCamera && PlayerDialogueCamera && PlayerFirstLookCamera))
    {
        PlayerFirstLookCamera->SetActive(false);
        PlayerDefaultCamera->SetActive(!IsDialogStarted);
        PlayerDialogueCamera->SetActive(IsDialogStarted);
    }
}
