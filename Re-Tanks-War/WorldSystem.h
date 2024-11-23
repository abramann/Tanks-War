#pragma once

#include "Subsystem.h"
#include "Renderer.h" // MeshData
#include <vector>

class IEntity;

class CWorldSystem : public ISubsystem
{
public:
	std::string getName() const override { return "WorldSystem"; }

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void shutdown() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pEntity) override;
	void unregisterComponent(ISystemComponent* pEntity) override;
	void handleEvent(ISystemComponent* pEntity, int eventCode, void* event) override;

	void setMap(const std::string& mapName);
	const MeshData& getWorldMesh();

private:
	void parseMap();

private:
	std::string m_mapName;
	std::vector<IEntity*> m_pEntities;
	MeshData m_mesh;
};

extern CWorldSystem* g_pWorldSystem;

