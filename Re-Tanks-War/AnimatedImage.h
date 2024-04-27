#pragma once

#include "Image.h"
#include <vector>




class ITexture;

class CAnimatedImage : public CImage
{
public:
	CAnimatedImage(const wchar_t* texFileName);
	virtual ~CAnimatedImage();
	
	ITexture* getTexture() const override;

	void makeAnimate(bool animate);
	void insertTexture(const wchar_t* texFileName, int order = -1);
	void eraseTexture(int place = -1);
	void makeConditionAnimate(bool *pCondition);
	void animateAt(int textureNumber = 0);
	void makeDelay(int milliseconds);

private:

	int m_waitPeriod;
	mutable int m_waited;
	mutable int64_t m_prevTime;
	bool m_animate;
	bool* m_pConditionAnimate;
	mutable int m_animateCounter;
	std::vector<ITexture*> m_pTextures;
};

