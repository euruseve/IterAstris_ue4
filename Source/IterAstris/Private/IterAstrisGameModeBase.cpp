// Copyright Epic Games, Inc. All Rights Reserved.

#include "IterAstrisGameModeBase.h"
#include "Player/IABaseCharacter.h"
#include "Player/IAPlayerController.h"
#include "UI/IAHUD.h"

AIterAstrisGameModeBase::AIterAstrisGameModeBase()
{
    DefaultPawnClass = AIABaseCharacter::StaticClass();
    PlayerControllerClass = AIAPlayerController::StaticClass();
    HUDClass = AIAHUD::StaticClass();
}