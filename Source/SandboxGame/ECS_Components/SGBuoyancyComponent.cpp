
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
void USGBuoyancyComponentWrapper::BeginPlay()
{
	Super::BeginPlay();
	RegisterComponentWithECS(IECSGameInstanceInterface::GetRegistry(this));
}

void USGBuoyancyComponentWrapper::RegisterComponentWithECS(entt::registry& Registry)
{
	IECSComponentWrapper::RegisterComponentWithECS(Registry);
	EntityHandle.AddComponent<FSGBuoyancyComponent>(DefaultValues);
}

//////////////////////////////////////////////////
void USGBuoyancyComponentWrapper::SyncWithECSComponent_Implementation()
{
	EntityHandle.GetComponent<FSGBuoyancyComponent>() = DefaultValues;
}

void USGBuoyancyComponentWrapper::UpdateECSComponent(const FSGBuoyancyComponent& NewValues)
{
	DefaultValues = NewValues;
	SyncWithECSComponent_Implementation();
}

//////////////////////////////////////////////////
FSGBuoyancyComponent& USGBuoyancyComponentWrapper::GetComponent() const
{
	return EntityHandle.GetComponent<FSGBuoyancyComponent>();
}
