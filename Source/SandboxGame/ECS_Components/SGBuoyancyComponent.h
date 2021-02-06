// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ECSComponentWrapperInterface.h"
#include "SGBuoyancyComponent.generated.h"


/**
 * Main component for the buoyancy system.
 * Uses a grid of line traces 
 */
USTRUCT(BlueprintType)
struct SANDBOXGAME_API FSGBuoyancyComponent
{
    GENERATED_BODY()
    
    /* Distance between line traces. Traces start along the centre line */
    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    FVector2D GridSpacing = { 50.0f, 50.0f };

    /* Trace along this axis (@see bInverseTraceDirection) */
    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    TEnumAsByte<EAxis::Type> TraceDirection = EAxis::Z;

    /* Should the trace direction be inverted? */
    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    bool bInverseTraceDirection = true;

    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
    bool bApplyDragForcesInWater = true;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float DragCoefficient = 20.f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float DragCoefficient2 = 0.01f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float AngularDragCoefficient = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float MaxDragSpeed = 15.f;

    /* Water bodies which we are currently overlapping */
    UPROPERTY(Transient)
    TArray<class AWaterBody*> CurrentWaterBodies;

    float GetWaterHeight(FVector Position, float DefaultHeight, bool bShouldIncludeWaves) const;
    float GetWaterHeight(FVector Position, float DefaultHeight, bool bShouldIncludeWaves, AWaterBody*& OutWaterBody) const;
    float GetWaterHeight(FVector Position, float DefaultHeight, AWaterBody*& OutWaterBody, float& OutWaterDepth,
                                               FVector& OutWaterPlaneLocation, FVector& OutWaterPlaneNormal,
                                               FVector& OutWaterSurfacePosition, FVector& OutWaterVelocity, int32& OutWaterBodyIdx,
                                               bool bShouldIncludeWaves) const;
};


/**
 * Blueprint component for the buoyancy system
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "SG Buoyancy Component"))
class SANDBOXGAME_API USGBuoyancyComponentWrapper : public UActorComponent, public IECSComponentWrapper
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
	
    virtual void RegisterComponentWithECS(entt::registry& Registry) override;
    virtual void SyncWithECSComponent_Implementation() override;

	/** Override the ECS version of this component with the new given values and sets @see DefaultValues */
    UFUNCTION(BlueprintCallable, Category = "ECS")
    void UpdateECSComponent(const FSGBuoyancyComponent& NewValues);

	/** Returns a reference to the ECS component */
    UFUNCTION(BlueprintPure, Category = "ECS")
    FSGBuoyancyComponent& GetComponent() const;
	
protected:
	/* Default parameters for this component. When changed after BeginPlay(), you have to call UpdateECSComponent() in order to copy the
	* new values to the ECS component. @see UpdateDefaultsFromECS() */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ECS")
	FSGBuoyancyComponent DefaultValues;
};
