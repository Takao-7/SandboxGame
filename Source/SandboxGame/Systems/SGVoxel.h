
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
			UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
			VoxelData = *VoxelizeMesh(MeshComponent, VoxelSize, VoxelIndexOffset);
			this->VoxelSize = VoxelSize;
			MeshRef = MeshComponent->GetStaticMesh();
			CenterOffset = MeshComponent->Bounds.Origin - MeshComponent->GetComponentLocation();
			
			CalculateVolume();

			UE_LOG(LogTemp, Warning, TEXT("%s: Voxel volume: %.1f"), *MeshComponent->GetStaticMesh()->GetName(), GetVolume_m3());
		}

		/**
		 * Iterate over every voxel
		 * Position is in voxel space
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
						FIntVector LocalPosition = Index + VoxelIndexOffset;
						Lambda(LocalPosition, VoxelData.GetValue(x, y, z, FVoxelValue::Empty()).ToFloat());
					}
				}
			}
		}

		/**
		* Iterate over every voxel in the given horizontal layer
		* Position is in voxel space
		*/
		void ForEach(TFunction<void (FIntVector Position, float Value)> Lambda, int32 Z) const
		{
			for (int32 x = 0; x < GetExtend().X; ++x)
			{
				for (int32 y = 0; y < GetExtend().Y; ++y)
				{
					FIntVector Index = { x, y, Z };
					FIntVector LocalPosition = Index + VoxelIndexOffset;
					Lambda(LocalPosition, VoxelData.GetValue(x, y, Z, FVoxelValue::Empty()).ToFloat());
				}
			}
		}

		/** Return the world location for a voxel position */
		FVector GetWorldLocation(const FIntVector& Position, const FTransform& Transform) const;

		void SetupGroups(float TargetGroupSize, bool bShowDebug = false, UStaticMeshComponent* MeshComp = nullptr);

		float GetVoxelSize() const
		{
			return VoxelSize;
		}

		FIntVector GetExtend() const
		{
			return VoxelData.GetSize();
		}

		FIntVector GetOffset() const { return VoxelIndexOffset; }

		/** Get the volume in cubic meters */
		float GetVolume_m3() const { return (Volume / FMath::Pow(100.0f, 3.0f)); }

		/** Get the volume in cubic centimeters */
		float GetVolume_cm3() const { return Volume; }

		float CalculateVolume();

		float operator[] (const FIntVector& Position) const
		{
			FIntVector Index = Position - GetOffset();
			return VoxelData.GetValue(Index.X, Index.Y, Index.Z, FVoxelValue::Empty()).ToFloat();
		}

		float GetValue(int32 X, int32 Y, int32 Z) const
		{
			return operator[]({ X, Y, Z });
		}

		const TArray<struct FVoxelGroup>& GetGroups() const { return VoxelGroups; }

	private:
		FVoxelDataAssetData VoxelData;

		/* Offset for converting a voxel position to the internal array coordinates */
		FIntVector VoxelIndexOffset;
	
		/* In cm^3 */
		float Volume = INDEX_NONE;
		float VoxelSize = 10.0f;

		/* Pointer to the mesh that we are based on (if any) */
		TSoftObjectPtr<UStaticMeshComponent> MeshRef;

		FVector CenterOffset = FVector::ZeroVector;

		TArray<struct FVoxelGroup> VoxelGroups;
	};

	
	//////////////////////////////////////////////////	
	//////////////////////////////////////////////////
	struct FVoxelGroup
	{
		FVoxelGroup(){}
		/**
		 * @param VoxelComponent
		 * @param Start				In voxel space
		 * @param End				In voxel space
		 */
		FVoxelGroup(const TVoxelComponent& VoxelComponent, FIntVector Start, FIntVector End)
		{			
			FVector SumWeightedVolumes = FVector::ZeroVector;
			for (int32 z = Start.Z; z < End.Z; ++z)
			{
				for (int32 x = Start.X; x < End.X; ++x)
				{
					for (int32 y = Start.Y; y < End.Y; ++y)
					{
						// A negative value is inside, so we have to convert it into a positive.
						// Values > 0 are outside the mesh, so they don't add to the volume
						FIntVector Index = FIntVector(x, y, z);
						float Value = VoxelComponent[Index];
						if (Value <= .0f)
						{
							Value = FMath::GetMappedRangeValueClamped({ -1.0f, .0f }, { 1.0f, .5f }, Value);

							float VoxelVolume = FMath::Pow(VoxelComponent.GetVoxelSize(), 3.0f) * Value;
							Volume += VoxelVolume;

							FVector Position = FVector(Index) * VoxelComponent.GetVoxelSize();
							SumWeightedVolumes += Position * VoxelVolume;
						}
					}
				}
			}

			if (Volume > .0f)
			{
				CenterOfVolume = (1.0f / Volume) * SumWeightedVolumes;				
			}
			else
			{
				CenterOfVolume = FVector(End - Start) * .5f + FVector(VoxelComponent.GetOffset());
				CenterOfVolume *= VoxelComponent.GetVoxelSize();
			}
		}

		/** Return a sphere radius, suitable for our volume */
		float GetSphereRadius() const;

		/* Relative to parent voxel component's center */
		FVector CenterOfVolume = -FVector::OneVector;

		/* Our location, relative to our parent voxel component's center */
		FIntVector RelativeLocation = FIntVector::ZeroValue;

		/* Actual volume */
		float Volume = .0f;
	};
}
