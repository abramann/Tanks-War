#pragma once

#include "UI.h"

class CStartUI : public UI
{
public:
	void update() override;
	void reset() override;

private:
	void beginUI() override;
	void drawUI() override;
	void endUI() override;
};

