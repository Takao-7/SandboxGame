// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Plugins/UnrealEngineECS/Source/UnrealEngineECS/Public/ECSIncludes.h"

struct FSGBuoyancyComponent;

namespace SandboxGameCoreSystems
{
	void BuoyancySystem(float DeltaSeconds, entt::registry& Registry);

	float CalculateBuoyancyForce(float Volume, const FVector& Velocity, const FSGBuoyancyComponent& BuoyancyComp);
	FVector CalculateLinearDragForce(const FVector& Velocity, const FSGBuoyancyComponent& BuoyancyComp);
	FVector CalculateAngularDragTorque(const FVector& AngularVelocity, const FSGBuoyancyComponent& BuoyancyComp);

    float ToKmh(float SpeedCms);
}
