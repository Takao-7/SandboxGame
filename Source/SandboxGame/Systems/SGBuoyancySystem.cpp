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
    TEXT("SG.debug.BuoyancySystem"),
    true,
    TEXT("Show debugging for the buoyancy system"),
    ECVF_Default
);

TAutoConsoleVariable<float> CVarWaterDensity
(
    TEXT("SG.WaterDensity"),
    1.0f,
    TEXT("Set the water density for buoyancy system"),
    ECVF_Cheat
);

//////////////////////////////////////////////////
void SandboxGameCoreSystems::BuoyancySystem(float DeltaSeconds, entt::registry& Registry)
{
  	SCOPE_CYCLE_COUNTER(STAT_BuoyancySystem);

    auto View = Registry.view<const FSGBuoyancyComponent, const FActorPtrComponent>();
    for (auto [Entity, Buoyancy, Actor] : View.each())
    {
        // Check if we can apply buoyancy
        UPrimitiveComponent* SimulatingComponent = Actor.IsValid() ? Cast<UPrimitiveComponent>(Actor->GetRootComponent()) : nullptr;
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

        const bool bDisplayDebug = CVar_DebugBuoyancySystem->GetBool();

    	VoxelizeActor(*Actor, Buoyancy.GridSpacing.X, .0f, Buoyancy.TraceChannel);

        // Step 1: Apply buoyancy forces at the grid points
        for (int32 x = -NumTracesX; x < NumTracesX; ++x)
        {
            for (int32 y = -NumTracesY; y < NumTracesY; ++y)
            {
                // Line traces to find surfaces
                FVector Start = FVector(x * Buoyancy.GridSpacing.X, y * Buoyancy.GridSpacing.Y, Bottom);
            	Start = BoundsOrigin + FVector(x * Buoyancy.GridSpacing.X, y * Buoyancy.GridSpacing.Y, .0f);
            	Start.Z -= BoundsExtent.Z;                
                FVector End = Start + FVector::UpVector * FMath::Abs(Bottom) * 2.0f;

                TArray<FHitResult> Hits;
            	// if (!ActorMultiHitLineTrace(*Actor, Hits, Start, End, Buoyancy.TraceChannel))
            	// {
            	// 	continue;
            	// }

            	// TArray<TEnumAsByte<EObjectTypeQuery>> Types =
            	// {
            	// 	UEngineTypes::ConvertToObjectType(SimulatingComponent->GetCollisionObjectType())
            	// };
            	// if (!UKismetSystemLibrary::LineTraceMultiForObjects(*Actor, Start, End, Types, true, TArray<AActor*>(),
            	// 													EDrawDebugTrace::ForOneFrame, Hits, false))
            	// {
            	// 	continue;
            	// }   
            	
            	// if (!UKismetSystemLibrary::BoxTraceMulti(*Actor, Start, End, FVector(Buoyancy.GridSpacing/2.0f, 1.0f),
            	// 	FRotator::ZeroRotator,
            	// 	UEngineTypes::ConvertToTraceType(Buoyancy.TraceChannel), false, TArray<AActor*>(),
            	// 	bDisplayDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
            	// 	Hits, false))
            	// {
            	// 	continue;
            	// }
 
                // Remove all hits which didn't hit us 
                for (int32 i = Hits.Num()-1; i >= 0; --i)
                {
                    if (Hits[i].GetActor() != *Actor)
                    {
                        Hits.RemoveAt(i);
                    }
                }
                
                if (bDisplayDebug)
                {
	                for (const FHitResult& Hit : Hits)
	                {
		                UKismetSystemLibrary::DrawDebugPoint(*Actor, Hit.ImpactPoint, 10.0f, FLinearColor::Red, .0f);
	                }
                }
            	
                // We should have a even number of hits. An odd number would mean we hit a plane and not an enclosed mesh
                if (Hits.Num() % 2 != 0)
                {
                    // UKismetSystemLibrary::PrintString(*Actor, FString::Printf(TEXT("Buoyancy System: We have an uneven number of "
                    //                                                                "trace hits for %s"), *Actor->GetName()),
                    //                                   true, false, FLinearColor::Yellow, .0f);
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

//////////////////////////////////////////////////
bool SandboxGameCoreSystems::ActorMultiHitLineTrace(AActor* Actor, TArray<FHitResult>& OutHits, FVector Start, FVector End,
													ECollisionChannel Channel)
{
	OutHits.Empty();
	
	const FVector Direction = (End - Start).GetSafeNormal();
	FHitResult FirstHit, SecondHit, LastHit;
	do
	{
		FirstHit = {};
		SecondHit = {};
		
		FCollisionQueryParams Params;
		Params.bFindInitialOverlaps = true;
		Params.bIgnoreTouches = true;
		Params.bTraceComplex = true;

		// Do two line trace in the opposite directions
		bool bFirstTraceHit = Actor->ActorLineTraceSingle(FirstHit, Start, End, Channel, Params);
		bool bSecondTraceHit = Actor->ActorLineTraceSingle(SecondHit, End, Start, Channel, Params);
		if (!bFirstTraceHit || !bSecondTraceHit)
		{
			break;
		}

		// If their impact points are close to each other, break, because we hit a plane
		if (FMath::IsNearlyEqual(FirstHit.ImpactPoint.Z, SecondHit.ImpactPoint.Z, 1.0f))
		{
			break;
		}
		
		OutHits.Add(FirstHit);
		OutHits.Add(SecondHit);		
	}
	while (false);

	// Sort hit results in ascending order by the impact point's vertical location
	OutHits.Sort([](const FHitResult& First, const FHitResult& Second){ return First.ImpactPoint.Z < Second.ImpactPoint.Z; });

	return OutHits.Num() > 0;
}

float SandboxGameCoreSystems::VoxelizeActor(AActor* Actor, float VoxelSize, float WaterLevel, ECollisionChannel Channel)
{
	FVector BoundsOrigin, BoundsExtent;
	Actor->GetActorBounds(true, BoundsOrigin, BoundsExtent);

	int32 TotalHits = 0;
	int32 HitsBelowWater = 0;
	for (int32 x = -BoundsExtent.X; x < BoundsExtent.X; x += VoxelSize)
	{
		for (int32 y = -BoundsExtent.Y; y < BoundsExtent.Y; y += VoxelSize)
		{
			for (int32 z = -BoundsExtent.Z; z < BoundsExtent.Z; z += VoxelSize)
			{
				UWorld* World = Actor->GetWorld();
				
				FVector BoxSize = FVector::OneVector * VoxelSize * .5f;
				FVector HalfBox = BoxSize;
				HalfBox.Z /= 2.0f;

				FVector VoxelLocation = FVector(x, y, z) + BoundsOrigin;
				FVector Start = VoxelLocation - FVector::UpVector * VoxelSize * .25f;
				FVector End = VoxelLocation + FVector::UpVector * VoxelSize * .25f;
				
				FCollisionQueryParams Params;
				Params.bFindInitialOverlaps = true;
				Params.bIgnoreTouches = false;
				Params.bTraceComplex = true;
				
				FHitResult Hit;
				if (World->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, Channel, FCollisionShape::MakeBox(BoxSize), Params))					
				{
					AActor* HitActor = Hit.GetActor();
					if (HitActor != Actor)
					{
						continue;
					}
					
					TotalHits++;
					bool bIsBelowWater = VoxelLocation.Z <= WaterLevel;
					if (bIsBelowWater)
					{
						HitsBelowWater++;
					}
					
					UKismetSystemLibrary::DrawDebugBox(Actor, VoxelLocation, BoxSize, bIsBelowWater ? FLinearColor::Green :
													   FLinearColor::White);

					UKismetSystemLibrary::DrawDebugArrow(World, Start, End, 5.0f, FLinearColor::Red);
					UKismetSystemLibrary::DrawDebugPoint(World, VoxelLocation, 10.0f, FLinearColor::Red);
				}
			}
		}
	}

	return HitsBelowWater * FMath::Pow(VoxelSize, 3);
}
