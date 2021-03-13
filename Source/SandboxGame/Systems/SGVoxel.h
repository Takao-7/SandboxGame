
#pragma once
#include "UEEnTTEntity.h"
#include "VoxelAssets/VoxelDataAssetData.h"


namespace SGVoxel
{
	template<class T>
    struct TVoxelArray
	{
		FIntVector Size = FIntVector(ForceInit);
		TArray<T> Data;

		TVoxelArray() = default;
		explicit TVoxelArray(const FIntVector& NewSize)
		{
			Resize(NewSize);
		}
	
		const T& operator()(const FIntVector& P) const
		{
			return (*this)(P.X, P.Y, P.Z);
		}
		T& operator()(const FIntVector& P)
		{
			return (*this)(P.X, P.Y, P.Z);
		}
	
		const T& operator()(int32 I, int32 J, int32 K) const
		{
			return const_cast<TVoxelArray<T>&>(*this)(I, J, K);
		}
		T& operator()(int32 I, int32 J, int32 K)
		{
			check(0 <= I && I < Size.X);
			check(0 <= J && J < Size.Y);
			check(0 <= K && K < Size.Z);
			return Data.GetData()[I + J * Size.X + K * Size.X * Size.Y];
		}

		void Resize(const FIntVector& NewSize)
		{
			check(static_cast<int64>(NewSize.X) * static_cast<int64>(NewSize.Y) * static_cast<int64>(NewSize.Z) < MAX_int32);
			Size = NewSize;
			Data.Empty(Size.X * Size.Y * Size.Z);
			Data.SetNumUninitialized(Size.X * Size.Y * Size.Z);
		}

		void Assign(const T& Value)
		{
			for (auto& X : Data)
			{
				X = Value;
			}
		}

		void Memzero()
		{
			FMemory::Memzero(Data.GetData(), Data.Num() * sizeof(T));
		}

		void ForEach(TFunction<void (FIntVector Position, float& Value)> Lambda)
		{
			for (int32 x = 0; x < Size.X; ++x)
			{
				for (int32 y = 0; y < Size.Y; ++y)
				{
					for (int32 z = 0; z < Size.Z; ++z)
					{
						FIntVector Position = {x, y, z};
						Lambda(Position, (*this)(Position));
					}
				}
			}
		}
	};

	
	//////////////////////////////////////////////////	
	//////////////////////////////////////////////////
	TVoxelSharedRef<FVoxelDataAssetData> VoxelizeMesh(class UStaticMeshComponent* MeshComponent, float VoxelSize,
													  FIntVector& OutPositionOffset);

	/** Group voxels by increasing the size of each voxel by the given reduction factor */
	struct TVoxelComponent GroupVoxels(const TVoxelComponent& InVoxelComp, float ReductionFactor);

	/**
	 * Transform a world location into a voxel array index.
	 * @param Location	The location in world space
	 * @param Entity	Entity to get the TVoxelComponent from. Also needs an FActorPtrComponent or FTransform component
	 */
	FIntVector WorldLocationToVoxelArray(FVector Location, FEntity Entity);

	/* Given a start and end location, return all voxel coordinates which are intersecting with the line */
	TArray<FIntVector> GetVoxelCoordinatesInLine(const FIntVector& TraceStart, const FIntVector& TraceEnd);

	/** Return all voxel values for the given coordinates (in voxel space) */
	TArray<float> GetVoxels(const TArray<FIntVector>& Coordinates, const struct TVoxelComponent& VoxelComponent);

	/**
	 * From:  http://www.cse.yorku.ca/~amana/research/grid.pdf
	 *
	 * @param Threshold		Any value smaller or equal than this is considered to be "solid" and return a hit 
	 */
	FIntVector VoxelLineTrace(float Threshold);
	

	//////////////////////////////////////////////////	
	//////////////////////////////////////////////////
	struct TVoxelComponent
	{
		TVoxelComponent(){}
		TVoxelComponent(const AActor* Actor, float VoxelSize)
		{
			VoxelData = *VoxelizeMesh(Actor->FindComponentByClass<UStaticMeshComponent>(), VoxelSize, PositionOffset); 
			this->VoxelSize = VoxelSize;
			CalculateVolume();
		}

		/**
		 * Iterate over every voxel
		 * Position is in voxel space and is NOT the index for the internal data array.
		 */
		void ForEach(TFunction<void (FIntVector Position, float Value)> Lambda) const
		{
			for (int32 z = 0; z < GetExtend().X; ++z)
			{
				for (int32 x = 0; x < GetExtend().X; ++x)
				{
					for (int32 y = 0; y < GetExtend().Y; ++y)
					{
						FIntVector Index = { x, y, z };
						FIntVector LocalPosition = Index + PositionOffset;
						Lambda(LocalPosition, VoxelData.GetValue(x, y, z, FVoxelValue::Empty()).ToFloat());
					}
				}
			}
		}

		/**
		* Iterate over every voxel in the given horizontal layer
		* Position is in voxel space and is NOT the index for the internal data array.
		*/
		void ForEach(TFunction<void (FIntVector Position, float Value)> Lambda, int32 Z) const
		{
			for (int32 x = 0; x < GetExtend().X; ++x)
			{
				for (int32 y = 0; y < GetExtend().Y; ++y)
				{
					FIntVector Index = { x, y, Z };
					FIntVector LocalPosition = Index + PositionOffset;
					Lambda(LocalPosition, VoxelData.GetValue(x, y, Z, FVoxelValue::Empty()).ToFloat());
				}
			}
		}

		/** Return the world location for a voxel position */
		FVector GetWorldLocation(const FIntVector& Position, const FTransform& Transform) const;

		void SetupGroups(float TargetGroupSize, bool bShowDebug = false, FTransform Transform = FTransform::Identity,
						 UObject* WorldContext = nullptr);

		/** Groups voxels together in groups */
		void CreateVoxelGroups();

		float GetVoxelSize() const
		{
			return VoxelSize;
		}

		FIntVector GetExtend() const
		{
			return VoxelData.GetSize();
		}

		/** Get the volume in cubic meters */
		float GetVolume_m3() const { return (Volume / FMath::Pow(100.0f, 3.0f)); };

		/** Get the volume in cubic centimeters */
		float GetVolume_cm3() const { return Volume; };

		float CalculateVolume();

		float operator[] (const FIntVector& Position) const
		{
			return VoxelData.GetValue(Position.X, Position.Y, Position.Z, FVoxelValue::Empty()).ToFloat();
		}

		FVoxelDataAssetData VoxelData;
		FIntVector PositionOffset;
	
	private:
		/* Volume in cm^3 */
		float Volume = INDEX_NONE;
		float VoxelSize = 10.0f;
		TArray<struct FVoxelGroup> VoxelGroups;
	};

	
	//////////////////////////////////////////////////	
	//////////////////////////////////////////////////
	struct FVoxelGroup
	{
		FVoxelGroup(){}
		FVoxelGroup(const TVoxelComponent& VoxelComponent, FIntVector Start, FIntVector End)
		{
			Extent = End - Start;
			
			FVector SumWeightedVolumes = FVector::ZeroVector;
			for (int32 z = Start.Z; z < End.Z; ++z)
			{
				for (int32 x = Start.X; x < End.X; ++x)
				{
					for (int32 y = Start.Y; y < End.Y; ++y)
					{
						FIntVector Index = { x, y, z };
						FIntVector LocalPosition = Index + VoxelComponent.PositionOffset;

						// A negative value is inside, so we have to convert it into a positive.
						// Values > 0 are outside the mesh, so they don't add to the volume 
						float Value = VoxelComponent.VoxelData.GetValue(x, y, z, FVoxelValue::Empty()).ToFloat();
						Value = -FMath::Min(Value, .0f);
						
						float Volume = FMath::Pow(VoxelComponent.GetVoxelSize(), 3.0f) * Value;
						TotalVolume += Volume;
						SumWeightedVolumes += FVector(LocalPosition) * Volume;
					}
				}
			}
			
			CenterOfVolume = (1.0f / TotalVolume) * SumWeightedVolumes;
		}
		
		FVector CenterOfVolume = -FVector::OneVector;
		FVector RelativeLocation = FVector::ZeroVector;

		/* Actual volume */
		float TotalVolume = .0f;

		/* Voxel size */
		float Size = INDEX_NONE;

		FIntVector Extent;
	};
}
