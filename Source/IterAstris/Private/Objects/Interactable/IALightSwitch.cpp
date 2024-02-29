// Iter Astris. All Rights Reserved.

#include "Objects/Interactable/IALightSwitch.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"

AIALightSwitch::AIALightSwitch()
{
    _Root = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    RootComponent = _Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    Mesh->SetupAttachment(GetRootComponent());

    PointLightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLight");
    PointLightComponent->SetupAttachment(GetRootComponent());

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(GetRootComponent());

}

void AIALightSwitch::BeginPlay()
{
	Super::BeginPlay();
    InteractionWidget->SetVisibility(false);
    PointLightComponent->SetIntensity(0.f);
}

void AIALightSwitch::Interact_Implementation() 
{
    UE_LOG(LogTemp, Display, TEXT("It works"));
    IsOn = !IsOn;
    PointLightComponent->SetIntensity(IsOn ? 10000.f : 0.f);
}

void AIALightSwitch::ShowInteractionWidget_Implementation() 
{
    InteractionWidget->SetVisibility(true);
}

void AIALightSwitch::HideInteractionWidget_Implementation() 
{
    InteractionWidget->SetVisibility(false);
}


