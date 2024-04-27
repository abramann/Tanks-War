#pragma once

#include "GameData.h"
#include "IImage.h"
#include "VecMath.h"
#include <iostream>




class ITexture;

class CImage : public IImage
{
	const bool DEFAULT_RELATIVE_CAMERA = true;
public:
	CImage(const wchar_t* texFileName);
	virtual ~CImage();

protected:
	void createVertexBuffer() override;
	void createIndexBuffer() override;
	void updateVertexBuffer() override;
	
	Rect8 makeVertexBufferData();
};