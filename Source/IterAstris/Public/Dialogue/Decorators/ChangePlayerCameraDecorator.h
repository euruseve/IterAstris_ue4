// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Decorators/MounteaDialogueDecoratorBase.h"
#include "ChangePlayerCameraDecorator.generated.h"

UCLASS()
class ITERASTRIS_API UChangePlayerCameraDecorator : public UMounteaDialogueDecoratorBase
{
    GENERATED_BODY()

public:
    virtual void InitializeDecorator_Implementation(
        UWorld* World, const TScriptInterface<IMounteaDialogueParticipantInterface>& OwningParticipant) override;
    virtual bool ValidateDecorator_Implementation(TArray<FText>& ValidationMessages) override;

    virtual void CleanupDecorator_Implementation() override;

private:
    APawn* PlayerPawn = nullptr;

    class UCameraComponent* PlayerDefaultCamera;
    class UCameraComponent* PlayerDialogueCamera;
    class UCameraComponent* PlayerFirstLookCamera;

    void ChangeCamera(bool IsDialogStarted);
};
