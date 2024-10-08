#pragma once

class UI
{
public:
	virtual void update() = 0;
	virtual UI* show() = 0;
	virtual void reset() = 0;
	
	UI* getParent() const { return m_pParent; }

private:
	UI* m_pParent;
};
