#pragma once

#include "Widget.h"

class ITexture;

class CImage : public IWidget
{
public:
	std::string getName() const override { return "Image"; };

	void update() override;
	void reset() override;
	void draw() override;

	void setTexture(ITexture* pTexture);

private:
	ITexture* m_pTexture = nullptr;
};

