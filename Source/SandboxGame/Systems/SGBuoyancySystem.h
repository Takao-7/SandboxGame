// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Plugins/UnrealEngineECS/Source/UnrealEngineECS/Public/ECSIncludes.h"

namespace SandboxGameCoreSystems
{
	void BuoyancySystem(float DeltaSeconds, entt::registry& Registry);

    float ToKmh(float SpeedCms);
}
