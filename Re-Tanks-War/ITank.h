#pragma once



class CTankModel;

class ITank
{
public:
	virtual ~ITank() {};

	//virtual void activeMovement(bool movemoent) = 0;
	//virtual void setVelocity(float velocity) = 0;
	
protected:
	bool m_movement;
	float m_velocity;
	float m_health;
	CTankModel* m_pTheModel;
};
