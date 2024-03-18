// Iter Astris. All Rights Reserved.

#include "Objects/Interactable/IASpaceInfoComputer.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Animations/IAComputerInteractionAnimNotify.h"
#include <Kismet/GameplayStatics.h>
#include <Player/IABaseCharacter.h>

DEFINE_LOG_CATEGORY_STATIC(LogSpaceInfoComp, All, All);

AIASpaceInfoComputer::AIASpaceInfoComputer()
{
    _Root = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    RootComponent = _Root;

    ComputerMesh = CreateDefaultSubobject<UStaticMeshComponent>("ComputerMesh");
    ComputerMesh->SetupAttachment(GetRootComponent());

    InteractionBox = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    InteractionBox->SetupAttachment(GetRootComponent());

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>("InteractionWidget");
    InteractionWidget->SetupAttachment(GetRootComponent());
}

void AIASpaceInfoComputer::BeginPlay()
{
    Super::BeginPlay();
    InteractionWidget->SetVisibility(false);

    InitAnimations();
}

void AIASpaceInfoComputer::Interact_Implementation()
{
    const auto Player = Cast<AIABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (!Player)
        return;

    Player->SetAnimationProgress(true);
    Player->PlayAnimation(Player->GetAnimations().ComputerInteractAnimMintage);
}

void AIASpaceInfoComputer::ShowInteractionWidget_Implementation()
{
    InteractionWidget->SetVisibility(true);
}

void AIASpaceInfoComputer::HideInteractionWidget_Implementation()
{
    InteractionWidget->SetVisibility(false);
}

void AIASpaceInfoComputer::InitAnimations()
{
    const auto Player = Cast<AIABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (!Player && !Player->GetAnimations().ComputerInteractAnimMintage)
        return;

    const auto NotifyEvents = Player->GetAnimations().ComputerInteractAnimMintage->Notifies;
    for (auto Event : NotifyEvents)
    {
        auto SuitChangeNotif = Cast<UIAComputerInteractionAnimNotify>(Event.Notify);
        if (SuitChangeNotif)
        {
            SuitChangeNotif->OnComputerInteractionNotified.AddUObject(
                this, &AIASpaceInfoComputer::OnInteraction);
            break;
        }
    }
}

void AIASpaceInfoComputer::OnInteraction(USkeletalMeshComponent* Mesh)
{
    const auto Player = Cast<AIABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!Player && !PlayerController)
        return;

    if (Player->GetMesh() != Mesh)
        return;
        
    Player->SetAnimationProgress(false);
    PlayerController->SetShowMouseCursor(true);
    PlayerController->SetInputMode(FInputModeUIOnly());
}