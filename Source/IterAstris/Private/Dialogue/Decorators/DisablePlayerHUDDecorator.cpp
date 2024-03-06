// Iter Astris. All Rights Reserved.

#include "Dialogue/Decorators/DisablePlayerHUDDecorator.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UDisablePlayerHUDDecorator::ValidateDecorator_Implementation(TArray<FText>& ValidationMessages)
{
    SetWidgetVisibility(ESlateVisibility::Hidden);

    return Super::ValidateDecorator_Implementation(ValidationMessages);
}

void UDisablePlayerHUDDecorator::CleanupDecorator_Implementation()
{
    SetWidgetVisibility(ESlateVisibility::Visible);
}

void UDisablePlayerHUDDecorator::SetWidgetVisibility(ESlateVisibility Visibility)
{
    if (GetWorld())
    {
        TArray<class UUserWidget*> Widgets;
        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, WidgetClass);
        for (auto Widget : Widgets)
        {
            Widget->SetVisibility(Visibility);
        }
    }
}
