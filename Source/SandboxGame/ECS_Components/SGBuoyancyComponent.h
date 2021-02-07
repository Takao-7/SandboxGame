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
    
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1.0f), Category = "Line tracing")
    float VoxelSize = 10.0f;

    /* Trace along this axis (@see bInverseTraceDirection) */
    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    TEnumAsByte<EAxis::Type> TraceDirection = EAxis::Z;

    /* Should the trace direction be inverted? */
    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
    bool bInverseTraceDirection = true;

    UPROPERTY(EditDefaultsOnly, Category = "Line tracing")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	/** Increases buoyant force applied */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyCoefficient = .1f;
	
	/** Damping factor to scale damping based on Z velocity. */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyDamp = 1000.0f;

	/**Second Order Damping factor to scale damping based on Z velocity. */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyDamp2 = 1.0f;
	
	/** Minimum velocity, in km/h, to start applying a ramp to buoyancy. */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyRampMinVelocity = 20.0f;

	/** Maximum velocity, in km/h until which the buoyancy can ramp up. */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyRampMaxVelocity = 500.0f;

	/** Maximum value that buoyancy can ramp to (at or beyond max velocity). */
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
	float BuoyancyRampMax = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy")
    bool bApplyDragForcesInWater = true;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float DragCoefficient = 20.f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float DragCoefficient2 = 0.01f;

    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float AngularDragCoefficient = 1.f;

	/* Max speed used to calculate drag force. In km/h */
    UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
    float MaxDragSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", Meta = (EditCondition = "bApplyDragForcesInWater"))
	float MaxBuoyantForce = 5000000.0f;

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
 * Blueprint component, used by the buoyancy system
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "ECS Buoyancy Component"))
class SANDBOXGAME_API UECSBuoyancyComponent : public UECSComponentWrapper
{
    GENERATED_BODY()

public:	
    virtual void BeginPlay() override;
    virtual void RegisterComponentWithECS() override;
	
	/** Override the ECS version of this component with the new given values and sets @see DefaultValues */
    UFUNCTION(BlueprintCallable, Category = "ECS")
    void UpdateECSComponent(const FSGBuoyancyComponent& NewValues);
	void UpdateECSComponent();

private:
	UFUNCTION()
	void OnOwnerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
    void OnOwnerEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	/* Default parameters for this component. When changed after BeginPlay(), you have to call UpdateECSComponent() in order to copy the
	* new values to the ECS component. @see UpdateDefaultsFromECS() */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ECS")
	FSGBuoyancyComponent DefaultValues;
};
