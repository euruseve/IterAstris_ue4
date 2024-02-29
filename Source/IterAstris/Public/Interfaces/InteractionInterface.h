// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


class ITERASTRIS_API IInteractionInterface
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Implementable")
    void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Implementable")
    void ShowInteractionWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Implementable")
    void HideInteractionWidget();
};
