// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, SandboxGame, "SandboxGame" );

float SGMath::NthRoot(float value, float degree)
{
	return FMath::Pow(value, (1.0f / degree));
}
