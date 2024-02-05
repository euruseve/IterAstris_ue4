// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IAPlayerHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class ITERASTRIS_API UIAPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetToxinLevel() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealth() const;
};
