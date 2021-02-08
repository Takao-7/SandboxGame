
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

TArray<float> SGVoxel::VoxelLineTrace(TVoxelComponent& VoxelComponent, const FVector& TraceStart, const FVector& TraceEnd,
									  float ValueThreshold)
{
	int32 x1 = TraceEnd.X, y1 = TraceEnd.Y, z1 = TraceEnd.Z, x0 = TraceStart.X, y0 = TraceStart.Y, z0 = TraceStart.Z;

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
	}

	return TArray<float>();
}

//////////////////////////////////////////////////
FVector SGVoxel::TVoxelComponent::GetWorldLocation(const FIntVector& Position, const FTransform& Transform) const
{
	return Transform.TransformPosition(FVector(Position * GetVoxelSize()));
}
