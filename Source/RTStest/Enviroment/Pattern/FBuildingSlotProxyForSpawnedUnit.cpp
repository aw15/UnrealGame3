#include "FSpawnedUnitBuildingSlotProxy.h"
#include "RTStest/Enviroment/BuildingSlot.h"

FSpawnedUnitBuildingSlotProxy FSpawnedUnitBuildingSlotProxy::NullProxyObject;

void FSpawnedUnitBuildingSlotProxy::DeepCopy(FSpawnedUnitBuildingSlotProxy& CopyTarget) const
{
	CopyTarget.SetBuildingSlot(BuildingSlotRef.Get());
}

void FSpawnedUnitBuildingSlotProxy::SetBuildingSlot(const ABuildingSlot* InBuildingSlot)
{
	BuildingSlotRef = InBuildingSlot;
}

const TArray<AMovableUnit*>& FSpawnedUnitBuildingSlotProxy::GetSpawnUnitList()
{
	return EmptySpawnUnitList; //TODO 정리필요
}

const AMovableUnit* FSpawnedUnitBuildingSlotProxy::GetLeaderUnit() const
{
	return nullptr; //TODO 정리필요
}

bool FSpawnedUnitBuildingSlotProxy::GetSpawnPointYaw(float& OutYaw) const
{
	if(BuildingSlotRef.IsValid())
	{
		return BuildingSlotRef->GetSpawnPointYaw(OutYaw);
	}

	return false;
}

bool FSpawnedUnitBuildingSlotProxy::IsValid() const
{
	return BuildingSlotRef.IsValid();
}
