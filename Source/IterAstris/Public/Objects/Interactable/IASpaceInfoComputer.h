// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "IASpaceInfoComputer.generated.h"

UCLASS()
class ITERASTRIS_API AIASpaceInfoComputer : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AIASpaceInfoComputer();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USceneComponent* _Root;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* ComputerMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UBoxComponent* InteractionBox;

    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    class UWidgetComponent* InteractionWidget;

    virtual void BeginPlay() override;

public:
    virtual void Interact_Implementation() override;
    virtual void ShowInteractionWidget_Implementation() override;
    virtual void HideInteractionWidget_Implementation() override;

private:
    void InitAnimations();

    UFUNCTION()
    void OnInteraction(USkeletalMeshComponent* Mesh);
};
