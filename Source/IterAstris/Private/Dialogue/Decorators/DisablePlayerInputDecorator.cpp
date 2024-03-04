// Iter Astris. All Rights Reserved.

#include "Dialogue/Decorators/DisablePlayerInputDecorator.h"
#include <Kismet/GameplayStatics.h>

void UDisablePlayerInputDecorator::InitializeDecorator_Implementation(
    UWorld* World, const TScriptInterface<IMounteaDialogueParticipantInterface>& OwningParticipant)
{
    Super::InitializeDecorator_Implementation(World, OwningParticipant);
    PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
    PlayerController = UGameplayStatics::GetPlayerController(World, 0);
}

bool UDisablePlayerInputDecorator::ValidateDecorator_Implementation(TArray<FText>& ValidationMessages)
{
    PlayerPawn->DisableInput(PlayerController);
    return Super::ValidateDecorator_Implementation(ValidationMessages);
}

void UDisablePlayerInputDecorator::CleanupDecorator_Implementation() 
{
    Super::CleanupDecorator_Implementation();
    PlayerPawn->EnableInput(PlayerController);
}
