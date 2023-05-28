#pragma once
#include "constants.h"
#include "clientplayer.h"
#include "serverplayer.h"
#include <memory>

class Game;
class Map2;
class TextureManger;
class Input;
class Timer;
class Net;

struct ClientData
{
	ClientData() {};
	ClientData(PlayerID id, const char* name, Game* game) { serverPlayer.initialize(id, name, game); }
	ServerPlayer serverPlayer;
	PlayerID getID() const { return serverPlayer.getID(); }
	void setID(PlayerID id) { serverPlayer.setID(id); }
	const char* getName() const { return serverPlayer.getName(); }
};

class Client
{
public:

	Client();
	~Client();
	bool connect();
	bool isConnected() const { return (m_state == CLIENT_CONNECTED_WAITING || m_state == CLIENT_CONNECTED_PLAYING) ? true : false; }
	char* getGameMap() { return (char*)m_map; }
	char* getPlayerName() { return m_clientInfo.name; }
	const char* getServerIP() { return m_clientInfo.serverIP; }
	const ClientState getState() const { return m_state; }
	const size_t getConnectedPlayers()const { return m_pClientData.size(); }
	const int8 getGamePlayers() const { return m_gamePlayers; }
	Port* getServerPort() { return &m_clientInfo.serverPort; }
	std::vector<std::shared_ptr<ClientData> > getClientData() { return m_pClientData; }
	void disconnect();
	void initialize(Game* game);
	void present();
	void update(float frameTime);
	ClientPlayer* getClientPlayer() const { return m_pClientPlayer.get(); }

private:

	bool recv();
	void addNewPlayer();
	void initPlayers();
	void playersUpdate();
	void playerUpdate();
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }
	void recv(bool wait);
	void removeClient(PlayerID id);
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void send(int size = MAX_PACKET_SIZE);
	void checkClientPlayerAct();
	void implementPlayerAct();
	ClientData* Client::getIDClient(const PlayerID id);

	TextureManger* m_pTextureManger;
	Audio* m_pAudio;
	Graphics* m_pGraphics;
	Input* m_pInput;
	Map2* m_pMap;
	Timer* m_pTimer;
	Game* m_pGame;

	Net m_net;
	Port m_port;
	ClientInfo m_clientInfo;
	int8_t m_gamePlayers;
	ClientState m_state;

	char m_map[MAX_NAME_LEN];

	CpsIni* m_pCpsIni;
	CpsDisconnect* m_pCpsDisconnect;
	CpsSeasson* m_pCpsSeasson;
	CpsPresent* m_pCpsPresent;
	CpsPlayerAct* m_pCpsPlayerAct;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersInitData* m_pSpsPlayerIniData;
	SpsPlayerUpdate* m_pSpsPlayerUpdate;
	SpsPlayerAct* m_pSpsPlayerAct;
	PacketType* m_pPacketType;

	float m_presentTime;
	char m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	std::vector<std::shared_ptr<ClientData> > m_pClientData;
	std::shared_ptr<ClientPlayer> m_pClientPlayer;

};
