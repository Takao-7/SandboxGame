// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBuoyancySystem.h"

#include "SGVoxel.h"
#include "SandboxGame/SandboxGame.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
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

/* Water density in g/cm^3 */
TAutoConsoleVariable<float> CVarWaterDensity
(
    TEXT("SG.WaterDensity"),
    1.0f,
    TEXT("Set the water density in g/cm^3 for buoyancy system"),
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

        const float VoxelSize = Buoyancy.VoxelSize;

    	float TotalVolume = .0f;
    	float TotalBuoyancy = .0f;
    	FVector AddedLocations = FVector::ZeroVector;

    	auto& VoxelComp = Registry.get_or_emplace<SGVoxel::TVoxelComponent>(Entity, *Actor, VoxelSize);
    	VoxelComp.ForEach([&](FIntVector Position, float Value)
        {    		
            if (Value == .0f)
            {
                return;
            }

            FVector VoxelLocation = VoxelComp.GetWorldLocation(Position, Actor->GetActorTransform());
            VoxelLocation += BoundsOrigin - Actor->GetActorLocation();
            // UKismetSystemLibrary::DrawDebugBox(*Actor, VoxelLocation, FVector::OneVector * VoxelSize * .5f, FLinearColor::Red,
      //           Actor->GetActorRotation());
      //
            float Volume = FMath::Pow(VoxelSize, 3.0f) * Value;
            TotalVolume += Volume;

            float WaterHeight = Buoyancy.GetWaterHeight(VoxelLocation, .0f, true);
            if (VoxelLocation.Z > WaterHeight)
            {
                AddedLocations += VoxelLocation * Volume;
                return;
            }
    		
            // Buoyancy formula:
            // Fb = Vs × D × g | Fb: Buoyancy force; Vs: Submerged volume; D = Fluid density; g: Gravity
            //	  <=> m^3 * kg/m^3 * m/s^2
            //	  <=> cm^3 * g/cm^3 * cm/s^2

            // Units: cm^3 * g/cm^3 * m/s^2 * 100 cm
            float BuoyancyForce = Volume * CVarWaterDensity->GetFloat() * (9.81f * 100.0f);
            BuoyancyForce /= 1000.0f; // g -> kg
            //SimulatingComponent->AddForceAtLocation(BuoyancyForce * FVector::UpVector, VoxelLocation);

            TotalBuoyancy += BuoyancyForce;
            AddedLocations += VoxelLocation * Volume;

            // float BuoyancyForce = CalculateBuoyancyForce(Volume, SimulatingComponent->GetPhysicsLinearVelocity(), Buoyancy);
            // BuoyancyForce = FMath::Clamp(BuoyancyForce, .0f, Buoyancy.MaxBuoyantForce);
            // SimulatingComponent->AddForceAtLocation(BuoyancyForce * FVector::UpVector, VoxelLocation);
        });

    	// Calculate the center of volume and apply the buoyancy force there
    	FVector CoV = (1.0f / TotalVolume) * AddedLocations;

    	// Clamp force
    	float GravityForce = 0.5f * SimulatingComponent->GetMass() * FMath::Square(9.81f);
    	GravityForce /= DeltaSeconds;
    	float ClampedBuoyancy = FMath::Clamp(TotalBuoyancy, .0f, 2 * GravityForce);
    	SimulatingComponent->AddForceAtLocation(ClampedBuoyancy * FVector::UpVector, CoV);

    	UKismetSystemLibrary::DrawDebugSphere(*Actor, CoV);
    	UKismetSystemLibrary::DrawDebugArrow(*Actor, Actor->GetActorLocation(), CoV, 2.0f, FLinearColor::Red, .0f, 2.0f);

  //   	if (Actor->GetActorLocation().Z < Buoyancy.GetWaterHeight(Actor->GetActorLocation(), .0f, true))
		// {
		// 	FVector Drag = CalculateLinearDragForce(SimulatingComponent->GetPhysicsLinearVelocity(), Buoyancy);
		// 	SimulatingComponent->AddForce(Drag, NAME_None, true);
	 //
		// 	FVector Torque = CalculateAngularDragTorque(SimulatingComponent->GetPhysicsAngularVelocityInDegrees(), Buoyancy);
		// 	SimulatingComponent->AddAngularImpulseInDegrees(Torque, NAME_None, true);
		// }

        float VolumeInCubicMeter = (TotalVolume / FMath::Pow(100.0f, 3.0f));
    	UKismetSystemLibrary::PrintString(*Actor, FString::Printf(TEXT("Buoyancy force: %.0f (kg*cm/s^2); Volume (m^3): %.1f"),
    															  TotalBuoyancy, VolumeInCubicMeter),
										  true, false, FLinearColor::Green, .0f);
    	
    	// Buoyancy formula:
    	// Fb = Vs × D × g | Fb: Buoyancy force; Vs: Submerged volume; D = Fluid density; g: Gravity
    	//	  = m^3 * g/cm^3 * m/s^2
    	// float BuoyancyForce = TotalSubmergedVolume * CVarWaterDensity->GetFloat() * (9.81f / 100.0f);
    	// BuoyancyForce = FMath::Clamp(BuoyancyForce, .0f, Buoyancy.MaxBuoyantForce);    	
    	// SimulatingComponent->AddForceAtLocation(BuoyancyForce * FVector::UpVector, BoundsOrigin);

    	// UKismetSystemLibrary::PrintString(*Actor, FString::Printf(TEXT("Total submerged volume (cm^3): %.0f"), TotalSubmergedVolume),
    	// 	true, false, FLinearColor::Green, .0f);
    	

        // Step 2: Handle drag
        // if (Buoyancy.bApplyDragForcesInWater)
        // {
        //     // Linear drag force
        //     FVector DragForce = FVector::ZeroVector;            
        //     FVector PlaneVelocity = SimulatingComponent->GetComponentVelocity();
        //     PlaneVelocity.Z = 0;
        //     const FVector VelocityDir = PlaneVelocity.GetSafeNormal();
        //     const float SpeedKmh = ToKmh(PlaneVelocity.Size());
        //     const float ClampedSpeed = FMath::Clamp(SpeedKmh, -Buoyancy.MaxDragSpeed, Buoyancy.MaxDragSpeed);
        //
        //     const float Resistance = ClampedSpeed * Buoyancy.DragCoefficient;
        //     DragForce += -Resistance * VelocityDir;
        //
        //     const float Resistance2 = ClampedSpeed * ClampedSpeed * Buoyancy.DragCoefficient2;
        //     DragForce += -Resistance2 * VelocityDir * FMath::Sign(SpeedKmh);
        //
        //     // Angular drag torque
        //     FVector DragTorque = -SimulatingComponent->GetPhysicsAngularVelocityInDegrees() * Buoyancy.AngularDragCoefficient;
        //
        //     // Apply drag forces
        //     SimulatingComponent->AddForce(DragForce, NAME_None, /*bAccelChange=*/true);
        //     SimulatingComponent->AddTorqueInDegrees(DragTorque, NAME_None, /*bAccelChange=*/true);
        // }

        if (TotalBuoyancy != .0f)
        {        	
    		// Drag equation:
    		// Fd = 0.5 * p * u^2 * Cd * A 
			// Fd: Drag force
			// p: Mass density of the fluid
			// u: Relative flow velocity;
    		// Cd: Drag coefficient
    		// A: Reference Area
	        FVector Velocity = SimulatingComponent->GetComponentVelocity();
    		float Speed = Velocity.Size();
        	Speed /= 100.0f; // cm -> m
    		float Area = BoundsExtent.X * BoundsExtent.Y;
        	Area /= FMath::Square(100.0f); // cm^2 -> m^2        	
    		float DragForce = 0.5f * CVarWaterDensity->GetFloat() * 1000.0f * FMath::Square(Speed) * Area;

        	// Clamp force
        	DragForce = FMath::Clamp(DragForce, .0f, Speed / DeltaSeconds);
        	
        	// kg * m / s^2 -> kg * cm / s^2
        	DragForce *= 100.0f;
   
    		SimulatingComponent->AddForce(-DragForce * Velocity.GetSafeNormal(), NAME_None, false);
   
        	FVector AngularVelocity = SimulatingComponent->GetPhysicsAngularVelocityInDegrees();
        	float AngularSpeed = AngularVelocity.Size();
        	AngularSpeed /= 100.0f; // cm -> m
        	float HorizontalArea = BoundsExtent.Z * BoundsExtent.Y;;
        	HorizontalArea /= FMath::Square(100.0f); // cm^2 -> m^2 
        	float DragTorque = 0.5f * CVarWaterDensity->GetFloat() * 1000.0f * FMath::Square(AngularSpeed) * HorizontalArea;

        	// kg * m / s^2 -> kg * cm / s^2
        	DragTorque *= 100.0f;
   
        	SimulatingComponent->AddTorqueInDegrees(-DragTorque * AngularVelocity.GetSafeNormal(), NAME_None, false);
        }
    }
}

//////////////////////////////////////////////////
float SandboxGameCoreSystems::CalculateBuoyancyForce(float Volume, const FVector& Velocity, const FSGBuoyancyComponent& BuoyancyComp)
{
	float ForwardSpeed = ToKmh(FVector2D(Velocity).Size());
	float VerticalSpeed = ToKmh(Velocity.Z);
	
	const float MinVelocity = BuoyancyComp.BuoyancyRampMinVelocity;
	const float MaxVelocity = BuoyancyComp.BuoyancyRampMaxVelocity;
	const float RampFactor = FMath::Clamp((ForwardSpeed - MinVelocity) / (MaxVelocity - MinVelocity), 0.f, 1.f);
	const float BuoyancyRamp = RampFactor * (BuoyancyComp.BuoyancyRampMax - 1);
	float BuoyancyCoefficientWithRamp = BuoyancyComp.BuoyancyCoefficient * (1 + BuoyancyRamp);
	
	const float FirstOrderDrag = BuoyancyComp.BuoyancyDamp * VerticalSpeed;
	const float SecondOrderDrag = FMath::Sign(VerticalSpeed) * BuoyancyComp.BuoyancyDamp2 * FMath::Square(VerticalSpeed);
	const float DampingFactor = -FMath::Max(FirstOrderDrag + SecondOrderDrag, 0.f);

	return Volume * BuoyancyCoefficientWithRamp + DampingFactor;
}

FVector SandboxGameCoreSystems::CalculateLinearDragForce(const FVector& Velocity, const FSGBuoyancyComponent& BuoyancyComp)
{
	FVector DragForce = FVector::ZeroVector;
	FVector PlaneVelocity = Velocity;
	PlaneVelocity.Z = 0;
	const FVector VelocityDir = PlaneVelocity.GetSafeNormal();
	const float SpeedKmh = ToKmh(PlaneVelocity.Size());
	const float ClampedSpeed = FMath::Clamp(SpeedKmh, -BuoyancyComp.MaxDragSpeed, BuoyancyComp.MaxDragSpeed);

	const float Resistance = ClampedSpeed * BuoyancyComp.DragCoefficient;
	DragForce += -Resistance * VelocityDir;

	const float Resistance2 = ClampedSpeed * ClampedSpeed * BuoyancyComp.DragCoefficient2;
	DragForce += -Resistance2 * VelocityDir * FMath::Sign(SpeedKmh);

	return DragForce;
}

FVector SandboxGameCoreSystems::CalculateAngularDragTorque(const FVector& AngularVelocity, const FSGBuoyancyComponent& BuoyancyComp)
{
	return -AngularVelocity * BuoyancyComp.AngularDragCoefficient;
}

//////////////////////////////////////////////////
float SandboxGameCoreSystems::ToKmh(float SpeedCms)
{
    return SpeedCms * 0.036f; //cm/s to km/h
}
