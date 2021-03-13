
#include "SGVoxel.h"

#include "VoxelSharedPtr.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VoxelImporters/VoxelMeshImporter.h"

//////////////////////////////////////////////////
TVoxelSharedRef<FVoxelDataAssetData> SGVoxel::VoxelizeMesh(UStaticMeshComponent* MeshComponent, float VoxelSize,
														   FIntVector& OutPositionOffset)
{
	FVoxelMeshImporterInputData MeshData;
	UVoxelMeshImporterLibrary::CreateMeshDataFromStaticMesh(MeshComponent->GetStaticMesh(), MeshData);
	
	FTransform Transform = MeshComponent->GetComponentTransform();
	Transform.SetTranslation(FVector::ZeroVector);
	FVoxelMeshImporterSettings Settings;
	Settings.VoxelSize = VoxelSize;

	FVoxelMeshImporterRenderTargetCache Cache;
	int32 NumLeaks;
	const auto VoxelData = MakeVoxelShared<FVoxelDataAssetData>();
	UVoxelMeshImporterLibrary::ConvertMeshToVoxels(MeshComponent, MeshData, Transform, Settings, Cache, *VoxelData,
												   OutPositionOffset, NumLeaks);
    
    return VoxelData;
}

//////////////////////////////////////////////////
FIntVector SGVoxel::WorldLocationToVoxelArray(FVector Location, FEntity Entity)
{
	auto& VoxelComp = Entity.GetComponent<TVoxelComponent>();
	return FIntVector();
}

//////////////////////////////////////////////////
TArray<FIntVector> SGVoxel::GetVoxelCoordinatesInLine(const FIntVector& TraceStart, const FIntVector& TraceEnd)
{
	// Current coordinates
	int32 x0 = TraceStart.X, y0 = TraceStart.Y, z0 = TraceStart.Z;

	// End coordinates
	int32 x1 = TraceEnd.X, y1 = TraceEnd.Y, z1 = TraceEnd.Z;

	int32 dx = abs(x1 - x0);
	int32 dy = abs(y1 - y0);
	int32 dz = abs(z1 - z0);

	int32 stepX = x0 < x1 ? 1 : -1;
	int32 stepY = y0 < y1 ? 1 : -1;
	int32 stepZ = z0 < z1 ? 1 : -1;

	float hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	float tMaxX = hypotenuse * 0.5f / dx;
	float tMaxY = hypotenuse * 0.5f / dy;
	float tMaxZ = hypotenuse * 0.5f / dz;
	float tDeltaX = hypotenuse / dx;
	float tDeltaY = hypotenuse / dy;
	float tDeltaZ = hypotenuse / dz;

	TArray<FIntVector> Coordinates;
	while (x0 != x1 || y0 != y1 || z0 != z1)
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			}
			else if (tMaxX > tMaxZ)
			{
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else
			{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
		else if (tMaxX > tMaxY)
		{
			if (tMaxY < tMaxZ)
			{
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
			}
			else if (tMaxY > tMaxZ)
			{
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else
			{
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			}
			else if (tMaxY > tMaxZ)
			{
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
			else
			{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
		Coordinates.Add({ x0, y0, z0 });
	}

	return Coordinates;
}

//////////////////////////////////////////////////
TArray<float> SGVoxel::GetVoxels(const TArray<FIntVector>& Coordinates, const TVoxelComponent& VoxelComponent)
{
	TArray<float> Values;
	for (auto& Coordinate : Coordinates)
	{
		VoxelComponent[Coordinate];
	}

	return Values;
}

//////////////////////////////////////////////////
FIntVector SGVoxel::VoxelLineTrace(float Threshold)
{
	// Step one: Initialization
	// Find the voxel in which the ray originates


	
	// Step two: Incremental traversal

	// Ray equation: U + tV for t >= 0

	// Incremental phase of the traversal algorithm
	int32 X = 0, Y = 0, Z = 0;

	// Step size. Initialized with 1 or -1, which is determined by the sign of the x and y components of V
	int32 stepX = 1;
	int32 stepY = 1;
	int32 stepZ = 1;

	float tMaxX = .0f, tMaxY = .0f, tMaxZ = .0f;
	float tDeltaX = .0f, tDeltaY = .0f, tDeltaZ = .0f;

	float justOutX = .0f, justOutY = .0f, justOutZ = .0f; 
	
	TOptional<FIntVector> ImpactPoint;
	do
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				X += stepX;

				if (X == justOutX)
				{
					return {}; // Outside grid
				}
				
				tMaxX += tDeltaX;
			}
			else
			{
				Z += stepZ;				
				if (Z == justOutZ)
				{
					return {};
				}
				
				tMaxZ += tDeltaZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				Y += stepY;
				if (Y == justOutY)
				{
					return {};
				}
				
				tMaxY += tDeltaY;
			}
			else
			{
				Z += stepZ;
				if(Z == justOutZ)
				{
					return {};
				}
				
				tMaxZ += tDeltaZ;
			}
		}
		//list = ObjectList[X][Y][Z];
	}
	while(!ImpactPoint.IsSet());

	return ImpactPoint.Get(FIntVector::ZeroValue);
}

//////////////////////////////////////////////////
FVector SGVoxel::TVoxelComponent::GetWorldLocation(const FIntVector& Position, const FTransform& Transform) const
{
	return Transform.TransformPosition(FVector(Position) * GetVoxelSize());
}

void SGVoxel::TVoxelComponent::SetupGroups(float TargetGroupSize, bool bShowDebug, FTransform Transform, UObject* WorldContext)
{
	if (VoxelGroups.Num() > 0)
	{
		if (WorldContext != nullptr && bShowDebug)
		{
			for (int32 i = 0; i < VoxelGroups.Num(); ++i)
			{
				const auto& Group = VoxelGroups[i];
				
				FVector Center;
				FVector Extent = FVector(Group.Extent) * Group.Size;
				FRotator Rotation;
				UKismetSystemLibrary::DrawDebugBox(WorldContext, Center, Extent, FLinearColor::Red, Rotation);
			}
		}
		return;
	}

	VoxelGroups.Empty();

	// This voxel component's total width, in cm
	FVector Width = FVector(GetExtend()) * VoxelSize;

	// VoxelSize == 10; Extend.X = 100
	// ==> Width = 1000 cm
	
	// How many groups we need per axis
	FIntVector GroupsPerAxis =
	{
		FMath::CeilToFloat(Width.X / TargetGroupSize),
		FMath::CeilToFloat(Width.Y / TargetGroupSize),
		FMath::CeilToFloat(Width.Z / TargetGroupSize)
	};

	// TargetGroupSize = 100 cm
	// ==> GroupsPerAxis = 1000 / 100 = 10

	/* How many of OUR voxels are represented by a single group */
	FVector VoxelsPerGroup = Width / (GroupsPerAxis * VoxelSize);

	// 1000 cm / (10 * 10) = 10	

	for (int32 x = 0; x < GroupsPerAxis.X; ++x)
	{
		for (int32 y = 0; y < GroupsPerAxis.Z; ++y)
		{
			for (int32 z = 0; z < GroupsPerAxis.Z; ++z)
			{
				FIntVector Start = FIntVector(FVector(x, y, z) * VoxelsPerGroup);
				FIntVector End = Start + FIntVector(FVector::OneVector * VoxelsPerGroup);
				FVoxelGroup Group = FVoxelGroup(*this, Start, End);
				VoxelGroups.Add(Group);
				//UE_LOG(LogTemp, Log, TEXT("Added group with volume %.1f cm^3"), Group.TotalVolume);
			}
		}
	}
}

//////////////////////////////////////////////////
float SGVoxel::TVoxelComponent::CalculateVolume()
{
	Volume = 0;
	for (auto VoxelValue : VoxelData.GetRawValues())
	{
		float Value = VoxelValue.ToFloat();
		if (Value <= .0f)
		{
			float VoxelVolume = FMath::Pow(VoxelSize, 3.0f) * UKismetMathLibrary::MapRangeClamped(Value, -1.0f, .0f, 1.0f, .5f);
			Volume += VoxelVolume;
		}						
	}

	return Volume;
}
