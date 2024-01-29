// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/IAIntoxicationComponent.h"
#include "IAPlayerIntoxicationComponent.generated.h"

UCLASS()
class ITERASTRIS_API UIAPlayerIntoxicationComponent : public UIAIntoxicationComponent
{
    GENERATED_BODY()

public:
    UIAPlayerIntoxicationComponent();

protected:
    virtual void UpdateToxinLevel();

    virtual void DecreaseToxinLevel();

    virtual void BeginPlay() override;

public:
    virtual void SetCurrentZoneToxinLevel(float NewToxinLvl);
    virtual void HandleZoneStateChanged(bool InZone) override;

};
