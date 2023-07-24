#pragma once
#include "Job.h"

class Room
{
private:
	map<uint64, PlayerRef> m_players;

	JobQueue m_jobs;

	// 직접 접근 불가능하도록 private
public:
	// 싱글쓰레드 환경으로 구현
	void Enter(PlayerRef _player);
	void Leave(PlayerRef _player);
	void Broadcast(SendBufferRef _sendBuffer);

public:
	// 멀티쓰레드 환경에서는 job으로 접근시켜야함
	void FlushJob();

	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* _memFunc)(Args...), Args... _args)
	{
		auto job = MakeShared<MemberJob<T, Ret, Args...>>(static_cast<T*>(this), _memFunc, _args...);
		m_jobs.Push(job);
	}
};

extern Room GRoom;