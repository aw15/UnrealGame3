#pragma once

class ABuildingSlot;

class FSpawnedUnitBuildingSlotProxy
{
public:
	FSpawnedUnitBuildingSlotProxy() = default;
	FSpawnedUnitBuildingSlotProxy(const FSpawnedUnitBuildingSlotProxy&) = delete;
	FSpawnedUnitBuildingSlotProxy& operator=(const FSpawnedUnitBuildingSlotProxy&) = delete;
	void DeepCopy(FSpawnedUnitBuildingSlotProxy& CopyTarget) const;
	void SetBuildingSlot(const ABuildingSlot* InBuildingSlot);
	const TArray<class AMovableUnit*>& GetSpawnUnitList();
	const AMovableUnit* GetLeaderUnit() const;
	bool GetSpawnPointYaw(float& OutYaw) const;
	bool IsValid() const;

public:
	static FSpawnedUnitBuildingSlotProxy NullProxyObject;

private:
	TArray<AMovableUnit*> EmptySpawnUnitList;
	TWeakObjectPtr<const ABuildingSlot> BuildingSlotRef = nullptr;
};
