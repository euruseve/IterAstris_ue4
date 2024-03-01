// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "IALightSwitch.generated.h"

UCLASS()
class ITERASTRIS_API AIALightSwitch : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AIALightSwitch();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Interact_Implementation() override;
    virtual void ShowInteractionWidget_Implementation() override;
    virtual void HideInteractionWidget_Implementation() override;


private:
    UPROPERTY(EditDefaultsOnly)
    USceneComponent* _Root;

    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, Category = "Light")
    class UPointLightComponent* PointLightComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    class UWidgetComponent* InteractionWidget;

    bool IsOn = false;
};
