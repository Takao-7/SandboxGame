// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBuoyancySystem.h"


#include "BuoyancyComponent.h"
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
TAutoConsoleVariable CVar_DebugBuoyancySystem
(
    TEXT("SG.debug.BuoyancySystem"),
    true,
    TEXT("Show debugging for the buoyancy system"),
    ECVF_Default
);

/* Water density in g/cm^3 */
TAutoConsoleVariable CVarWaterDensity
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

    	auto& VoxelComp = Registry.get_or_emplace<SGVoxel::TVoxelComponent>(Entity, *Actor, Buoyancy.VoxelSize);
        UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
    	VoxelComp.SetupGroups(Buoyancy.PontoonSize, CVar_DebugBuoyancySystem->GetBool(), MeshComp);

        if (UBuoyancyComponent* BuoyancyComponent = Actor->FindComponentByClass<UBuoyancyComponent>())
        {
	        if (!BuoyancyComponent->HasPontoons())
	        {
	        	float TotalVolume = .0f;
	        	for (auto& Group : VoxelComp.GetGroups())
	        	{
		            if (Group.Volume > 0)
		            {
	        			BuoyancyComponent->AddCustomPontoon(Group.GetSphereRadius(), Group.CenterOfVolume);
		            	TotalVolume += Group.Volume;
		            }
	        	}
	        	BuoyancyComponent->UpdatePontoonCoefficients();

	        	UE_LOG(LogTemp, Warning, TEXT("%s: Total buoyancy volume: %.1f m^3. PontoonSize: %.0f"),
	        		   *MeshComp->GetStaticMesh()->GetName(), TotalVolume / FMath::Pow(100.0f, 3.0f), Buoyancy.PontoonSize);
	        }
        }
    }
}

//////////////////////////////////////////////////
float SandboxGameCoreSystems::ToKmh(float SpeedCms)
{
    return SpeedCms * 0.036f; //cm/s to km/h
}
