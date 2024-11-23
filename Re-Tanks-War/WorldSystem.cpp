#include "WorldSystem.h"
#include "DiskLoader.h"

static CWorldSystem worldSystem;
CWorldSystem* g_pWorldSystem = &worldSystem;

void CWorldSystem::startup()
{
}

void CWorldSystem::update()
{
}

void CWorldSystem::reset()
{
}

void CWorldSystem::perform()
{
}

void CWorldSystem::shutdown()
{
}

void CWorldSystem::onStartGame()
{
	parseMap();
}

void CWorldSystem::onQuitGame()
{
}

void CWorldSystem::onPauseGame()
{
}

void CWorldSystem::onResumGame()
{
}

void CWorldSystem::registerComponent(ISystemComponent* pEntity)
{
}

void CWorldSystem::unregisterComponent(ISystemComponent* pEntity)
{
}

void CWorldSystem::handleEvent(ISystemComponent* pEntity, int eventCode, void* event)
{
}

void CWorldSystem::setMap(const std::string& mapName)
{
	m_mapName = mapName;
}

const MeshData& CWorldSystem::getWorldMesh()
{
	return m_mesh;
}

void CWorldSystem::parseMap()
{
}
