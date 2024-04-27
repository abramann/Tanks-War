#pragma once

#include "IObject2D.h"
#include "Color.h"
#include "VecMath.h"


class IBuffer;
class ITexture;

class CTankModel : public IObject2D
{
public:
	CTankModel();
	virtual ~CTankModel();

	virtual ITexture* const * getChainsTextures(int& num) const;
	virtual ITexture* getBodyTexture() const;
	virtual ITexture* getGunTexture() const;
	virtual void makeScaling(Vertex scaling);
	virtual void setScaling(Vertex newScaling);
	virtual Vertex getScaling() const;
	virtual void setColor(Color newColor);
	virtual Color getColor() const;
	virtual float getBodyRotate() const;
	virtual void makeBodyRotate(float degree);
	virtual void setBodyRotate(float newRotateDegree);
	virtual float getGunRotate() const;
	virtual void makeGunRotate(float degree);
	virtual void setGunRotate(float newRotateDegree);
	virtual void makeToTargetGunRotate(Vertex target);
	virtual void setBodyRotateCenter(Vertex newPosition);
	virtual Vertex getBodyRotateCenter() const;
	virtual void setGunRotateCenter(Vertex newPosition);
	virtual Vertex getGunRotateCenter() const;

	void assignBodyTex(ITexture* pBodyTex);
	void assignGunTex(ITexture* pGunTex);
	void assignChainsTexs(ITexture** ppChainsTexs, int num);
	void assginVertexBuffer(IBuffer* pVertexBuffer);
	void assginIndexBuffer(IBuffer* pIndexBuffer);

protected:
	void createVertexBuffer() override;
	void createIndexBuffer() override;
	void updateVertexBuffer() override;

	ITexture* m_pBodyTex;
	ITexture* m_pGunTex;
	ITexture** m_ppChainsTexs;
	int m_numChainsTexs;
	Color m_color;
	Vertex m_scaling;
	float m_bodyRotateDegree;
	float m_gunRotateDegree;
	Vertex m_bodyRotateCenterPosition;
	Vertex m_gunRotateCenterPosition;
};

