#pragma once
#include "Job.h"

class Room
{
private:
	map<uint64, PlayerRef> m_players;

	JobQueue m_jobs;

	// ���� ���� �Ұ����ϵ��� private
public:
	// �̱۾����� ȯ������ ����
	void Enter(PlayerRef _player);
	void Leave(PlayerRef _player);
	void Broadcast(SendBufferRef _sendBuffer);

public:
	// ��Ƽ������ ȯ�濡���� job���� ���ٽ��Ѿ���
	void FlushJob();

	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* _memFunc)(Args...), Args... _args)
	{
		auto job = MakeShared<MemberJob<T, Ret, Args...>>(static_cast<T*>(this), _memFunc, _args...);
		m_jobs.Push(job);
	}
};

extern Room GRoom;