#ifndef _TEXTDX_H
#define _TEXTDX_H
#include "graphics.h"

class TextDX
{
public:

	TextDX();
	~TextDX();
	virtual bool initialize(int height, bool bold, bool italic, std::string fontFamily, Graphics* g);
	virtual void printf(int x, int y, std::string& text);
	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void release();

	virtual void setColor(COLOR _color)		{ color = _color; }
	virtual void setAngle(float _angle)		{ angle = _angle; }
	
	virtual COLOR getColor()		{ return color; }
	virtual float getAngle()		{ return angle; }

private:

	Graphics* graphics;

	LPD3DXFONT dxFont;
	RECT rect;
	D3DXMATRIX matrix;
	COLOR color;
	float angle;
};


#endif

