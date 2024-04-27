#include "AnimatedImage.h"
#include "IRenderer.h"
#include "ITimer.h"


CAnimatedImage::CAnimatedImage(const wchar_t* texFileName) : CImage::CImage(texFileName)
{
	m_animateCounter = 0;
	m_pConditionAnimate = nullptr;
	m_animate = true;
	m_waitPeriod = 400;
	m_prevTime = 0;
	m_pTextures.push_back(m_pTexture);
}

CAnimatedImage::~CAnimatedImage()
{
	for (auto pTexture : m_pTextures)
	{
		if (pTexture)
			g_pRenderer->releaseTexture(pTexture);
	}
	CImage::~CImage();
}

ITexture* CAnimatedImage::getTexture() const
{
	int64_t now = g_pTimer->getTime();
	m_waited += static_cast<int>(now - m_prevTime);
	m_prevTime = now;
	
	if (m_waited >= m_waitPeriod && ( (m_pConditionAnimate && *m_pConditionAnimate) || m_animate) )
	{
		ITexture* pTexture = m_pTextures[m_animateCounter++];
		if (m_animateCounter >= static_cast<int>(m_pTextures.size()))
		{
			m_animateCounter = 0;
		}
		m_waited = 0;
		return pTexture;
	}
	return m_pTextures[m_animateCounter];
}

void CAnimatedImage::makeAnimate(bool animate)
{
	m_animate = animate;
}

void CAnimatedImage::insertTexture(const wchar_t * texFileName, int order)
{
	ITexture* pTexture = g_pRenderer->loadTextureFromFile(texFileName);
	if (order == -1)
	{
		m_pTextures.push_back(pTexture);
	}
	else
	{
		m_pTextures.insert(m_pTextures.begin() + order, pTexture);
	}
}

void CAnimatedImage::eraseTexture(int place)
{
	if (place == -1)
	{
		m_pTextures.pop_back();
	}
	else
	{
		m_pTextures.erase(m_pTextures.begin() + place);
	}
}

void CAnimatedImage::makeConditionAnimate(bool * pCondition)
{
	m_pConditionAnimate = pCondition;
	if (pCondition)
	{
		m_animate = false;
	}
}

void CAnimatedImage::animateAt(int textureNumber)
{
	m_animateCounter = textureNumber;
}

void CAnimatedImage::makeDelay(int milliseconds)
{
	m_waitPeriod = milliseconds;
}
