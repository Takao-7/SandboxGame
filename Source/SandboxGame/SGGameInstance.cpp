// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameInstance.h"
#include "Systems/SGBuoyancySystem.h"

void USGGameInstance::Init()
{
    Super::Init();
    AddSystem(SandboxGameCoreSystems::BuoyancySystem, ESystemTickingGroup::PrePhysics);
}
