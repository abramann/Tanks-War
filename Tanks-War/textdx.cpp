#include "TextDX.h"

TextDX::TextDX()
{
	dxFont = NULL;
	color = COLOR_ARGB(0, 0, 0, 0);
	angle = 0;
	ZeroMemory(&rect, sizeof(RECT));
	ZeroMemory(&matrix, sizeof(D3DMATRIX));
}

TextDX::~TextDX()
{
	release();
}

bool TextDX::initialize(int height, bool bold, bool italic, std::string fontFamily, Graphics* g)
{
	graphics = g;
	WORD weight;
	if (bold)
		weight = FW_BOLD;
	else
		weight = FW_NORMAL;

	HRESULT result = D3DXCreateFontA(graphics->getDevice(), height, 0, weight,
		1, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontFamily.c_str(), &dxFont);
		if (FAILED(result))
			return false;

		rect.right = GAME_WIDTH;
		rect.bottom = GAME_HEIGHT;
		return true;
}

void TextDX::printf(int x, int y, std::string& text)
{
	graphics->spriteBegin();
	rect.left = x;	
	rect.top = y;
	V2 rCenter = V2(x, y);
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
	graphics->getSprite()->SetTransform(&matrix);
	dxFont->DrawTextA(graphics->getSprite(), text.c_str(), -1, &rect, DT_LEFT, color);
	graphics->spriteEnd();
}

void TextDX::onLostDevice()
{
	dxFont->OnLostDevice();
}

void TextDX::onResetDevice()
{
	dxFont->OnResetDevice();
}

void TextDX::release()
{
	SAFE_RELEASE(dxFont);
}