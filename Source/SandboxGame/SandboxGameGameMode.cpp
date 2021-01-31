// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxGameGameMode.h"
#include "SandboxGameHUD.h"
#include "SandboxGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASandboxGameGameMode::ASandboxGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASandboxGameHUD::StaticClass();
}
