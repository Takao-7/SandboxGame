
#include "SGBuoyancyComponent.h"
#include "WaterBodyActor.h"

float FSGBuoyancyComponent::GetWaterHeight(FVector Position, float DefaultHeight, bool bShouldIncludeWaves) const
{
    AWaterBody* DummyActor;
    return GetWaterHeight(Position, DefaultHeight, bShouldIncludeWaves, DummyActor);
}

float FSGBuoyancyComponent::GetWaterHeight(FVector Position, float DefaultHeight, bool bShouldIncludeWaves, AWaterBody*& OutWaterBody) const
{
    float DummyDepth;
    FVector DummyWaterPlaneLocation;
    FVector DummyWaterPlaneNormal;
    FVector DummyWaterSurfacePosition;
    FVector DummyWaterVelocity;
    int32 DummyWaterBodyIndex;
    return GetWaterHeight(Position, DefaultHeight, OutWaterBody, DummyDepth, DummyWaterPlaneLocation, DummyWaterPlaneNormal,
                       DummyWaterSurfacePosition, DummyWaterVelocity, DummyWaterBodyIndex, bShouldIncludeWaves);
}

float FSGBuoyancyComponent::GetWaterHeight(FVector Position, float DefaultHeight,
                                           AWaterBody*& OutWaterBody, float& OutWaterDepth, FVector& OutWaterPlaneLocation,
                                           FVector& OutWaterPlaneNormal, FVector& OutWaterSurfacePosition, FVector& OutWaterVelocity,
                                           int32& OutWaterBodyIdx, bool bShouldIncludeWaves) const
{
    float WaterHeight = DefaultHeight;
    OutWaterBody = nullptr;
    OutWaterDepth = 0.f;
    OutWaterPlaneLocation = FVector::ZeroVector;
    OutWaterPlaneNormal = FVector::UpVector;

    float MaxImmersionDepth = -1.f;
    for (AWaterBody* CurrentWaterBody : CurrentWaterBodies)
    {
        if (CurrentWaterBody)
        {
            EWaterBodyQueryFlags QueryFlags =
                EWaterBodyQueryFlags::ComputeLocation
                | EWaterBodyQueryFlags::ComputeNormal
                | EWaterBodyQueryFlags::ComputeImmersionDepth
                | EWaterBodyQueryFlags::ComputeVelocity;

            if (bShouldIncludeWaves)
            {
                QueryFlags |= EWaterBodyQueryFlags::IncludeWaves;
            }

            FWaterBodyQueryResult QueryResult = CurrentWaterBody->QueryWaterInfoClosestToWorldLocation(Position, QueryFlags);
            if (QueryResult.IsInWater() && QueryResult.GetImmersionDepth() > MaxImmersionDepth)
            {
                check(!QueryResult.IsInExclusionVolume());
                WaterHeight = Position.Z + QueryResult.GetImmersionDepth();
                OutWaterBody = CurrentWaterBody;
                if (EnumHasAnyFlags(QueryResult.GetQueryFlags(), EWaterBodyQueryFlags::ComputeDepth))
                {
                    OutWaterDepth = QueryResult.GetWaterSurfaceDepth();
                }
                OutWaterPlaneLocation = QueryResult.GetWaterPlaneLocation();
                OutWaterPlaneNormal = QueryResult.GetWaterPlaneNormal();
                OutWaterSurfacePosition = QueryResult.GetWaterSurfaceLocation();
                OutWaterVelocity = QueryResult.GetVelocity();
                OutWaterBodyIdx = CurrentWaterBody ? CurrentWaterBody->WaterBodyIndex : 0;
                MaxImmersionDepth = QueryResult.GetImmersionDepth();
            }
        }
    }
    return WaterHeight;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
void UECSBuoyancyComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UECSBuoyancyComponent::OnOwnerBeginOverlap);
	GetOwner()->OnActorEndOverlap.AddDynamic(this, &UECSBuoyancyComponent::OnOwnerEndOverlap);

	TArray<AActor*> Overlaps;
	GetOwner()->GetOverlappingActors(Overlaps, AWaterBody::StaticClass());
	for (AActor* WaterBody : Overlaps)
	{
		OnOwnerBeginOverlap(GetOwner(), WaterBody);
	}
}

void UECSBuoyancyComponent::RegisterComponentWithECS()
{
	Super::RegisterComponentWithECS();
	EntityHandle.AddComponent<FSGBuoyancyComponent>(DefaultValues);
}

//////////////////////////////////////////////////
void UECSBuoyancyComponent::UpdateECSComponent(const FSGBuoyancyComponent& NewValues)
{
	DefaultValues = NewValues;
	EntityHandle.GetComponent<FSGBuoyancyComponent>() = NewValues;
}

void UECSBuoyancyComponent::UpdateECSComponent()
{
	EntityHandle.GetComponent<FSGBuoyancyComponent>() = DefaultValues;
}

//////////////////////////////////////////////////
void UECSBuoyancyComponent::OnOwnerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AWaterBody* WaterBody = Cast<AWaterBody>(OtherActor))
	{
		DefaultValues.CurrentWaterBodies.AddUnique(WaterBody);
		UpdateECSComponent();
	}
}

void UECSBuoyancyComponent::OnOwnerEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AWaterBody* WaterBody = Cast<AWaterBody>(OtherActor))
	{
		DefaultValues.CurrentWaterBodies.RemoveSwap(WaterBody);
		UpdateECSComponent();
	}
}
