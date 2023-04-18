#include "tank.h"
#include "fileio.h"

Tank::Tank() : m_client(0), m_remoteClient(0), m_playerAct(PLAYER_ACT_NONE)
{
}

Tank::~Tank()
{
	release();
}

void Tank::initialize(
	int width, int height, float health, float speed, Map* map,
	Texture* death, Texture* texture, Audio* audio, Graphics* graphics)
{
	Unit::initialize(width, height, 1, 1, false, 0, health, speed, map, death, texture, audio, graphics);
}

void Tank::initialize(Map * map, TextureManger * textureManger, Texture * texture, Audio *audio, Graphics * graphics)
{
	Unit::initialize(map, textureManger, texture, audio, graphics);
	m_tankInfo = *textureManger->getTextureInfo(texture->getNumber())->tankInfo;
	m_pFire = m_pTextureManger->getTexture(m_tankInfo.fireTexture);
	m_fire.initialize(this, m_pMap, m_pTextureManger, m_pFire, m_pGraphics);
	m_lpVertexBuffer = m_pGraphics->createVertexBuffer(6 * sizeof(Vertex), VB_USAGE_WRITEONLY);
	float width = m_spriteData.width*1.0f, height = m_spriteData.height*1.0f;
	Vertex v[6];
	v[0] = Vertex(-width, -height, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-width, height, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(width, height, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(-width, -height, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(width, height, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(width, -height, 0.0f, 1.0f, 1.0f);
	m_pGraphics->setVertexBuffer(m_lpVertexBuffer, v, 5);
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key,
	Key attack_key)
{
	Unit::inputInitialize(input, forward_key, back_key, right_key, left_key);
	m_key.push_back(attack_key);
}

void Tank::audioInitialize(Sound forward, Sound back, Sound right, Sound left, Sound death, Sound attack, Sound attacked)
{
	Unit::audioInitialize(forward, back, right, left, death);
	m_sound.push_back(attack);
	m_sound.push_back(attacked);
}

void Tank::inputUpdate(float frameTime)
{
	if (m_remoteClient)
		return;

	m_playerAct = PLAYER_ACT_NONE;
	Object::inputUpdate(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_TANK_ATTACK]))
		executeAttack(frameTime);
}

void Tank::update(float frameTime)
{
	Object::update(frameTime);
	//m_fire.update(frameTime);
}

void Tank::draw()
{
	float width = m_spriteData.width*1.0f, height = m_spriteData.height*1.0f;

	m_pGraphics->streamVertexBuffer(m_lpVertexBuffer);
	m_pGraphics->setTexture(m_pTextureManger->getTexture(2)->getTexture());
	//m_pGraphics->drawPrimitive(0, 8, V3(m_spriteData.x*1.0f, m_spriteData.y*1.0f, -2.0f));
	Object::draw();
	if (m_fire.active())
		m_fire.draw();
}

void Tank::release()
{
}

void Tank::executeForward(float frameTime)
{
	m_playerAct = PLAYER_ACT_FORWRAD;
	if (m_client)
		return;

	float x = getX() + (m_objectInfo.speed * m_sinA);
	float y = getY() - (m_objectInfo.speed * m_cosA);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
	y = m_pMap->passY(getX(), y, getY(), getHeight());
	setY(y);

	if (m_playAudio)
		m_pAudio->play(m_sound[SOUND_TANK_FORWARD]);
}

void Tank::executeBack(float frameTime)
{
	m_playerAct = PLAYER_ACT_BACK;
	if (m_client)
		return;

	float x = getX() - (m_objectInfo.speed * m_sinA);
	float y = getY()+ (m_objectInfo.speed * m_cosA);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
	y = m_pMap->passY(getX(), y, getY(), getHeight());
	setY(y);

	if (m_playAudio)
		m_pAudio->play(m_sound[SOUND_TANK_BACK]);
}

void Tank::executeRight(float frameTime)
{
	m_playerAct = PLAYER_ACT_RIGHT;
	if (m_client)
		return;

	if (m_spriteData.angle >= 3.14)
		m_spriteData.angle *= -1;
	
	m_spriteData.angle += TANK_ROTATE_AMOUNT;
	if (m_playAudio)
		m_pAudio->play(SOUND_TANK_RIGHT);
}

void Tank::executeLeft(float frameTime)
{
	m_playerAct = PLAYER_ACT_LEFT;
	if (m_client)
		return;

	if (m_spriteData.angle <= - PI)
		m_spriteData.angle *= -1;
	
	m_spriteData.angle -= TANK_ROTATE_AMOUNT;
	if (m_playAudio)
		m_pAudio->play(SOUND_TANK_LEFT);
}

void Tank::executeAttack(float frameTime)
{
	m_playerAct = PLAYER_ACT_ATTACK;
	if (m_client)
		return;

	if (m_fire.active())
		return;

	m_fire.release(RELEASE_NORMAL);
	if (m_playAudio)
		m_pAudio->play(SOUND_TANK_ATTACK);

}

void Tank::executeAttacked(float damage)
{
	m_objectInfo.health -= damage;
	if (m_playAudio)
		m_pAudio->play(SOUND_TANK_ATTACKED);
}

void Tank::setTexture(Texture* texture, Texture* fire)
{
	Unit::setTexture(texture);
	m_pFire = fire;
}

void Tank::endFrame()
{
	Object::endFrame();
}

void Tank::applyPlayerUpdate(const PlayerUpdate playerUpdate)
{
	setX(playerUpdate.x);
	setY(playerUpdate.y);
	setAngle(playerUpdate.angle);
	setHealth(playerUpdate.health);
}

PlayerUpdate Tank::getPlayerUpdate() const
{
	PlayerUpdate playerUpdate;
	playerUpdate.x = getX();
	playerUpdate.y = getY();
	playerUpdate.angle = getAngle();
	playerUpdate.health = getHealth();
	return playerUpdate;
}
