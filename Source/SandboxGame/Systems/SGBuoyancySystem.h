// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Plugins/UnrealEngineECS/Source/UnrealEngineECS/Public/ECSIncludes.h"

namespace SandboxGameCoreSystems
{
	void BuoyancySystem(float DeltaSeconds, entt::registry& Registry);

    float ToKmh(float SpeedCms);

	
	bool ActorMultiHitLineTrace(AActor* Actor, TArray<FHitResult>& OutHits, FVector Start, FVector End, ECollisionChannel Channel);

	/**
	 * @return Volume below water level
	 */
	float VoxelizeActor(AActor* Actor, float VoxelSize, float WaterLevel, ECollisionChannel Channel);
}
