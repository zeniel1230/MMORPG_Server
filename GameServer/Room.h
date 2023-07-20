#pragma once
#include "Job.h"

class Room
{
	friend class EnterJob;
	friend class LeaveJob;
	friend class BroadcastJob;

private:
	map<uint64, PlayerRef> m_players;

	JobQueue m_jobs;

	// 직접 접근 불가능하도록 private
private:
	// 싱글쓰레드 환경으로 구현
	void Enter(PlayerRef _player);
	void Leave(PlayerRef _player);
	void Broadcast(SendBufferRef _sendBuffer);

public:
	// 멀티쓰레드 환경에서는 job으로 접근시켜야함
	void PushJob(JobRef job) { m_jobs.Push(job); }
	void FlushJob();
};

extern Room GRoom;

// Room Jobs
class EnterJob : public IJob
{
public:
	Room& m_room;
	PlayerRef m_player;

public:
	EnterJob(Room& _room, PlayerRef _player) : m_room(_room), m_player(_player)
	{
	}

	virtual void Execute() override
	{
		m_room.Enter(m_player);
	}
};

class LeaveJob : public IJob
{
public:
	Room& m_room;
	PlayerRef m_player;

public:
	LeaveJob(Room& _room, PlayerRef _player) : m_room(_room), m_player(_player)
	{
	}

	virtual void Execute() override
	{
		m_room.Leave(m_player);
	}
};

class BroadcastJob : public IJob
{
public:
	Room& m_room;
	SendBufferRef m_sendBuffer;

public:
	BroadcastJob(Room& _room, SendBufferRef _sendBuffer) : m_room(_room), m_sendBuffer(_sendBuffer)
	{
	}

	virtual void Execute() override
	{
		m_room.Broadcast(m_sendBuffer);
	}
};