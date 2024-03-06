// Iter Astris. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Decorators/MounteaDialogueDecoratorBase.h"
#include "DisablePlayerHUDDecorator.generated.h"

UCLASS()
class ITERASTRIS_API UDisablePlayerHUDDecorator : public UMounteaDialogueDecoratorBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
    TSubclassOf<class UUserWidget> WidgetClass;

    virtual bool ValidateDecorator_Implementation(TArray<FText>& ValidationMessages) override;

    virtual void CleanupDecorator_Implementation() override;

private:
    void SetWidgetVisibility(ESlateVisibility Visibility);
};
