#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Á÷Á¢ ÄÁÅÙÃ÷ ÀÛ¾÷ÀÚ

bool Handle_INVALID(PacketSessionRef& _session, BYTE* _buffer, int32 _len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);
	// TODO : Log
	return false;
}

bool Handle_C_TEST(PacketSessionRef& _session, Protocol::C_TEST& _pkt)
{
	// TODO

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	return true;
}