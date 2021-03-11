
#include "SGVoxel.h"
#include "Kismet/KismetSystemLibrary.h"

SGVoxel::TVoxelArray<float> SGVoxel::VoxelizeActor(const AActor* Actor, float VoxelSize)
{
	FVector BoundsOrigin, BoundsExtent;
	Actor->GetActorBounds(true, BoundsOrigin, BoundsExtent);

	FIntVector VoxelBounds = FIntVector((BoundsExtent / VoxelSize));
	VoxelBounds += FIntVector(1);
	UKismetSystemLibrary::DrawDebugBox(Actor, BoundsOrigin, BoundsExtent, FLinearColor::Yellow, Actor->GetActorRotation());

	// Initialize the output array
	TVoxelArray<float> VoxelArray (VoxelBounds * 2);
	float TotalVolume = .0f;

	for (int32 x = -VoxelBounds.X; x < VoxelBounds.X; ++x)
	{
		for (int32 y = -VoxelBounds.Y; y < VoxelBounds.Y; ++y)
		{
			for (int32 z = -VoxelBounds.Z; z < VoxelBounds.Z; ++z)
			{
				FVector BoxSize = FVector::OneVector * VoxelSize * .5f;
				FVector HalfBox = BoxSize;
				HalfBox.Z /= 2.0f;

				const FTransform& ActorTransform = Actor->GetActorTransform();

				//FVector VoxelLocation = ActorTransform.TransformPosition(FVector(x, y, z) * VoxelSize);
				FVector VoxelLocation = ActorTransform.TransformVector(FVector(x, y, z) * VoxelSize) + BoundsOrigin;
				FVector TraceOffset = ActorTransform.TransformVector(FVector::UpVector * VoxelSize * .25f);
				FVector Start = VoxelLocation - TraceOffset;
				FVector End = VoxelLocation + TraceOffset;
				FRotator Rotation = Actor->GetActorRotation();
				
				FCollisionQueryParams Params;
				Params.bFindInitialOverlaps = true;
				Params.bIgnoreBlocks = false;
				Params.bIgnoreTouches = false;

				UWorld* World = Actor->GetWorld();
				ECollisionChannel CollisionType = Actor->GetRootComponent()->GetCollisionObjectType();
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(CollisionType) };

				FIntVector VoxelArrayLocation = { x + VoxelBounds.X, y + VoxelBounds.Y, z + VoxelBounds.Z };
				float Value = .0f;
				TArray<FHitResult> Hits;
				if (World->SweepMultiByObjectType(Hits, Start, End, Rotation.Quaternion(),
					FCollisionObjectQueryParams::DefaultObjectQueryParam,
					FCollisionShape::MakeBox(HalfBox), Params))
				// if (UKismetSystemLibrary::BoxTraceMultiForObjects(World, Start, End, HalfBox, Rotation, ObjectTypes, true,
				// 												  TArray<AActor*>(), EDrawDebugTrace::None, Hits, false))
				{
					if (FHitResult* Hit = Hits.FindByPredicate([&](const FHitResult& Other)
					{
						return Other.GetComponent() == Actor->GetRootComponent();
					}))
					{
						FHitResult TraceHit;
						bool bTraceHit = World->LineTraceSingleByObjectType(TraceHit, VoxelLocation, Hit->ImpactPoint,
                            FCollisionObjectQueryParams::DefaultObjectQueryParam, Params);
						if (bTraceHit && TraceHit.bStartPenetrating)
						{
							Value = 1.0f;
						}
						else
						{
							float Distance = FVector::Dist(VoxelLocation, Hit->ImpactPoint);
							Value = FMath::GetMappedRangeValueClamped({ .0f, VoxelSize }, { 1.0f, .0f }, Distance);
						}
						
						TotalVolume += Value * FMath::Pow(VoxelSize, 3.0f);
					}					
				}
				
				VoxelArray(VoxelArrayLocation) = Value;

				// FLinearColor BoxColor = Value < 1.0f ? FLinearColor::Red : FLinearColor::White;
				// UKismetSystemLibrary::DrawDebugBox(Actor, VoxelLocation, BoxSize, BoxColor, Actor->GetActorRotation());
				// if (Value < 1.0f)
				// {
				// 	UKismetSystemLibrary::DrawDebugBox(Actor, VoxelLocation, BoxSize, FLinearColor::Red, Actor->GetActorRotation());
				// }
			}
		}
	}

	float VolumeInCubicMeter = (TotalVolume / FMath::Pow(100.0f, 3.0f));
	UE_LOG(LogTemp, Warning, TEXT("Total volume for '%s': %.2f m^3"), *Actor->GetName(), VolumeInCubicMeter);
	return VoxelArray;
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
	return Transform.TransformPosition(FVector(Position * GetVoxelSize()));
}

void SGVoxel::TVoxelComponent::SetupGroups(float TargetGroupSize, bool bShowDebug, UObject* WorldContext)
{
	if (VoxelGroups.Num() > 0 && FMath::IsNearlyEqual(VoxelGroups[0].Size, TargetGroupSize, .1f))
	{
		if (WorldContext != nullptr && bShowDebug)
		{
			for (int32 i = 0; i < VoxelGroups.Num(); ++i)
			{
				FVector Center;
				FVector Extent;
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
	FVector GroupsPerAxis =
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
				VoxelGroups.Add(FVoxelGroup(*this, Start, End));
			}
		}
	}
}
