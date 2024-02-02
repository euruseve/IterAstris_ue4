// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IAHUD.generated.h"

UCLASS()
class ITERASTRIS_API AIAHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD();

private:
    void DrawCrossHair();
};
