#pragma once

#include "Subsystem.h"
#include <vector>

class CSprite;
class IModel;
struct Mesh;

class CRenderSystem : public ISubsystem
{
public:
	enum RenderType
	{
		MODEL,
		SPRITE,
		WORLD,
		UI
	};

	CRenderSystem();

	std::string getName() const override { return "RenderSystem"; }

	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
	void shutdown() override;
	void onStartGame() override;
	void onQuitGame() override;
	void onPauseGame() override;
	void onResumGame() override;
	void registerComponent(ISystemComponent* pRenderComp) override;
	void unregisterComponent(ISystemComponent* pRenderComp) override;
	void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) override;

	void setRenderState(RenderType renderType, bool state);

private:
	void renderSprite(CSprite* pSprite);
	void renderModel(IModel* pModel);
	void renderUI(bool state); 
	void renderWorld();

	RenderType getRenderType(ISystemComponent* pComponent) const;

private:
	std::vector<class IRenderComponent*> m_pRenderedComponents;
	bool m_doRenderSprites;
	bool m_doRenderModels;
	bool m_doRenderMap;
	bool m_doRenderUI;
};

extern CRenderSystem* g_pRenderSystem;
