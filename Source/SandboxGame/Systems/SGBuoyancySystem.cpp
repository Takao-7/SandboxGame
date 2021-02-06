// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBuoyancySystem.h"
#include "SandboxGame/SandboxGame.h"

#include "Kismet/KismetSystemLibrary.h"
#include "WaterBodyActor.h"

#include "UEEnTTComponents.h"
#include "SandboxGame/ECS_Components/SGBuoyancyComponent.h"

//////////////////////////////////////////////////
DECLARE_CYCLE_STAT(TEXT("Buoycancy System"), STAT_BuoyancySystem, STATGROUP_SGCoreSystems);

//////////////////////////////////////////////////
TAutoConsoleVariable<bool> CVar_DebugBuoyancySystem
(
    TEXT("sb.debug.BuoyancySystem"),
    false,
    TEXT("Show debugging for the buoyancy system"),
    ECVF_Default
);

TAutoConsoleVariable<float> CVarWaterDensity
(
    TEXT("sb.WaterDensity"),
    1.0f,
    TEXT("Set the water density for buoyancy system"),
    ECVF_Cheat
);

//////////////////////////////////////////////////
void SandboxGameCoreSystems::BuoyancySystem(float DeltaSeconds, entt::registry& Registry)
{
  	SCOPE_CYCLE_COUNTER(STAT_BuoyancySystem);

    auto Group = Registry.group<const FSGBuoyancyComponent, const FActorPtrComponent>();
    for (auto [Entity, Buoyancy, Actor] : Group.each())
    {
        // Check if we can apply buoyancy
        UPrimitiveComponent* SimulatingComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (SimulatingComponent == nullptr)
        {
            return;
        }
    	
        const ECollisionEnabled::Type Collision = SimulatingComponent->GetCollisionEnabled();
        bool bCanApplyBuoyancy = SimulatingComponent->IsSimulatingPhysics() &&
                                 (Collision == ECollisionEnabled::QueryAndPhysics || Collision == ECollisionEnabled::PhysicsOnly);
        if (!bCanApplyBuoyancy)
        {
            continue;
        }
        
        FVector BoundsOrigin, BoundsExtent;
        Actor->GetActorBounds(true, BoundsOrigin, BoundsExtent);
        int32 NumTracesX = (BoundsExtent.X / Buoyancy.GridSpacing.X) * 2;
        int32 NumTracesY = (BoundsExtent.Y / Buoyancy.GridSpacing.Y) * 2;
        float Bottom = -BoundsExtent.Z;

        // Step 1: Apply buoyancy forces at the grid points
        for (int32 x = -NumTracesX; x < NumTracesX; ++x)
        {
            for (int32 y = -NumTracesY; y < NumTracesY; ++y)
            {
                // Line traces to find surfaces
                FVector Start = FVector(x * Buoyancy.GridSpacing.X, y * Buoyancy.GridSpacing.Y, Bottom);
                Start = Actor->GetActorTransform().TransformPosition(Start);
                FVector End = Start + FVector::UpVector * FMath::Abs(Bottom) * 2.0f;
                FCollisionQueryParams Params;                
                Params.bDebugQuery = CVar_DebugBuoyancySystem->GetBool();
                TArray<FHitResult> Hits;
                if (!Actor->GetWorld()->LineTraceMultiByChannel(Hits, Start, End, Buoyancy.TraceChannel, Params))
                {
                    continue;
                }
                
                // Remove all hits which didn't hit us 
                for (int32 i = Hits.Num()-1; i >= 0; --i)
                {
                    if (Hits[i].GetActor() != *Actor)
                    {
                        Hits.RemoveAt(i);
                    }
                }
                
                // We should have a even number of hits. An odd number would mean we hit a plane and not an enclosed mesh
                if (Hits.Num() % 2 != 0)
                {
                    UKismetSystemLibrary::PrintString(*Actor, FString::Printf(TEXT("Buoyancy System: We have an uneven number of "
                                                                                   "trace hits for %s"), *Actor->GetName()),
                                                      true, false, FLinearColor::Yellow, .0f);
                    continue;
                }

                // Check which points are below the water surface
                float WaterHeight = Buoyancy.GetWaterHeight(Start, .0f, true);

                // Iterate over pairs of hits: Each pair is the mesh thickness at this location
                for (int32 i = 0; i+1 < Hits.Num(); i += 2)
                {
                    const FVector_NetQuantize& LowerPoint = Hits[i].ImpactPoint;
                    if (LowerPoint.Z > WaterHeight)
                    {
                        continue; // The entire stack is above the waterline
                    }

                    // If the upper point is above the waterline, clamp it to the water height
                    FVector UpperPoint = Hits[i+1].ImpactPoint;
                    UpperPoint.Z = FMath::Min(UpperPoint.Z, WaterHeight);
                    float StackHeight = FVector::Dist(LowerPoint, UpperPoint);
                    float StackVolume = Buoyancy.GridSpacing.X * Buoyancy.GridSpacing.Y * StackHeight;

                    // Buoyancy formula:
                    // Fb = Vs × D × g | Fb: Buoyancy force; Vs: Submerged volume; D = Fluid density; g: Gravity
                    FVector BuoyancyForce = StackVolume * CVarWaterDensity->GetFloat() * 9.81f * FVector::UpVector;
                    SimulatingComponent->AddForceAtLocation(BuoyancyForce, LowerPoint);
                }
            }
        }

        // Step 2: Handle drag
        if (Buoyancy.bApplyDragForcesInWater)
        {
            // Linear drag force
            FVector DragForce = FVector::ZeroVector;            
            FVector PlaneVelocity = SimulatingComponent->GetComponentVelocity();
            PlaneVelocity.Z = 0;
            const FVector VelocityDir = PlaneVelocity.GetSafeNormal();
            const float SpeedKmh = ToKmh(PlaneVelocity.Size());
            const float ClampedSpeed = FMath::Clamp(SpeedKmh, -Buoyancy.MaxDragSpeed, Buoyancy.MaxDragSpeed);

            const float Resistance = ClampedSpeed * Buoyancy.DragCoefficient;
            DragForce += -Resistance * VelocityDir;

            const float Resistance2 = ClampedSpeed * ClampedSpeed * Buoyancy.DragCoefficient2;
            DragForce += -Resistance2 * VelocityDir * FMath::Sign(SpeedKmh);

            // Angular drag torque
            FVector DragTorque = -SimulatingComponent->GetPhysicsAngularVelocityInDegrees() * Buoyancy.AngularDragCoefficient;

            // Apply drag forces
            SimulatingComponent->AddForce(DragForce, NAME_None, /*bAccelChange=*/true);
            SimulatingComponent->AddTorqueInDegrees(DragTorque, NAME_None, /*bAccelChange=*/true);
        }
    }
}

//////////////////////////////////////////////////
float SandboxGameCoreSystems::ToKmh(float SpeedCms)
{
    return SpeedCms * 0.036f; //cm/s to km/h
}
