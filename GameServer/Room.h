#pragma once

class Room
{
private:
	USE_LOCK;
	map<uint64, PlayerRef> m_players;

public:
	void Enter(PlayerRef _player);
	void Leave(PlayerRef _player);
	void Broadcast(SendBufferRef _sendBuffer);
};

extern Room GRoom;