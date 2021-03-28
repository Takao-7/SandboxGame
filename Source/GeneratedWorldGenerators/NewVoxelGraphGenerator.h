// Copyright 2020 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelGeneratedWorldGeneratorsIncludes.h"
#include "NewVoxelGraphGenerator.generated.h"

UCLASS(Blueprintable)
class UNewVoxelGraphGenerator : public UVoxelGraphGeneratorHelper
{
	GENERATED_BODY()
	
public:
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Default Material"))
	TSoftObjectPtr<UMaterialInterface> Default_Material = TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath("/Voxel/Examples/Shared/Textures/TextureHaven/Rock/MI_Rock.MI_Rock"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Height"))
	float Height = 500.0;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Islands"))
	TSoftObjectPtr<UCurveFloat> Islands = TSoftObjectPtr<UCurveFloat>(FSoftObjectPath("/Game/VoxelGeneration/CRV_Islands.CRV_Islands"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Land"))
	TSoftObjectPtr<UCurveFloat> Land = TSoftObjectPtr<UCurveFloat>(FSoftObjectPath("/Game/VoxelGeneration/CRV_Land.CRV_Land"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Mountain erosion strength"))
	float Mountain_erosion_strength = 0.04;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Mountains"))
	TSoftObjectPtr<UCurveFloat> Mountains = TSoftObjectPtr<UCurveFloat>(FSoftObjectPath("/Game/VoxelGeneration/CRV_Mountains.CRV_Mountains"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Ocean"))
	TSoftObjectPtr<UCurveFloat> Ocean = TSoftObjectPtr<UCurveFloat>(FSoftObjectPath("/Game/VoxelGeneration/CRV_Ocean.CRV_Ocean"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Snow"))
	TSoftObjectPtr<UMaterialInterface> Snow = TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath("/Voxel/Examples/Shared/Textures/TextureHaven/Snow/MI_Snow.MI_Snow"));
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Snow Height"))
	float Snow_Height = 400.0;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(DisplayName="Valleys Height"))
	float Valleys_Height = -0.5;
	
	UNewVoxelGraphGenerator();
	virtual TVoxelSharedRef<FVoxelTransformableGeneratorInstance> GetTransformableInstance() override;
};
