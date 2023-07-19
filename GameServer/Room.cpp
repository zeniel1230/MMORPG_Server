#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

Room GRoom;

void Room::Enter(PlayerRef _player)
{
	WRITE_LOCK;
	m_players[_player->playerId] = _player;
}

void Room::Leave(PlayerRef _player)
{
	WRITE_LOCK;
	m_players.erase(_player->playerId);
}

void Room::Broadcast(SendBufferRef _sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : m_players)
	{
		p.second->ownerSession->Send(_sendBuffer);
	}
}