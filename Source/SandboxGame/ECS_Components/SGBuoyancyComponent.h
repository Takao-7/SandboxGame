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
    
    UPROPERTY(EditAnywhere, meta = (ClampMin = 1.0f))
    float VoxelSize = 10.0f;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.0f))
	float PontoonSize = 100.0f;

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
