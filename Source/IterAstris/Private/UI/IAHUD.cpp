// Iter Astris. All Rights Reserved.

#include "UI/IAHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void AIAHUD::BeginPlay()
{
    Super::BeginPlay();
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void AIAHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrossHair();
}

void AIAHUD::DrawCrossHair()
{
    const TInterval<float> Center{Canvas->SizeX * .5f, Canvas->SizeY * .5f};

    const float HalfLineSize = 8.f;
    const float LineThickness = 2.f;
    const FLinearColor LineColor = FLinearColor::Black;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
    
}
