#pragma once

#include "Subsystem.h"
#include <vector>

class CSprite;
class CMesh;

class CRenderSystem : public ISubsystem
{
	enum RenderComp
	{
		MODEL,
		SPRITE
	};

public:
	CRenderSystem();

	std::string getName() const override { return "RenderSystem"; }

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pRenderComp) override;
	void unregisterComponent(ISystemComponent* pRenderComp) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

private:
	RenderComp getRenderType(ISystemComponent* pComponent) const;

	std::vector<class IRenderComponent*> m_pRenderedComponents;
	bool m_renderSprites;
	bool m_renderObjects;
	bool m_renderMap;
	bool m_renderUI;
};

extern CRenderSystem* g_pRenderSystem;
