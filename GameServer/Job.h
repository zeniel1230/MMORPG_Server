#pragma once

// C++11 apply
template<int... Remains>
struct seq
{};

template<int N, int... Remains>
struct gen_seq : gen_seq<N - 1, N - 1, Remains...>
{};

template<int... Remains>
struct gen_seq<0, Remains...> : seq<Remains...>
{};

template<typename Ret, typename... Args>
void xapply(Ret(*_func)(Args...), std::tuple<Args...>& _tup)
{
	xapply_helper(_func, gen_seq<sizeof...(Args)>(), _tup);
}

template<typename F, typename... Args, int... ls>
void xapply_helper(F _func, seq<ls...>, std::tuple<Args...>& _tup)
{
	(_func)(std::get<ls>(_tup)...);
}

template<typename T, typename Ret, typename... Args>
void xapply(T* _obj, Ret(T::* _func)(Args...), std::tuple<Args...>& _tup)
{
	xapply_helper(_obj, _func, gen_seq<sizeof...(Args)>(), _tup);
}

template<typename T, typename F, typename... Args, int... ls>
void xapply_helper(T* _obj, F _func, seq<ls...>, std::tuple<Args...>& _tup)
{
	(_obj->*_func)(std::get<ls>(_tup)...);
}

class IJob
{
public:
	virtual void Execute() { }
};

template<typename Ret, typename... Args>
class FuncJob : public IJob
{
	using FuncType = Ret(*)(Args...);

public:
	FuncJob(FuncType _func, Args... _args) : m_func(_func), m_tuple(_args...)
	{

	}

	virtual void Execute() override
	{
		std::apply(m_func, m_tuple); // C++17
		//xapply(_func, _tuple);
	}
private:
	FuncType m_func;
	std::tuple<Args...> m_tuple;
};

template<typename T, typename Ret, typename... Args>
class MemberJob : public IJob
{
	using FuncType = Ret(T::*)(Args...);

public:
	MemberJob(T* _obj, FuncType _func, Args... _args) : m_obj(_obj), m_func(_func), m_tuple(_args...)
	{

	}

	virtual void Execute() override
	{
		//std::apply(m_func, m_tuple); // C++17
		xapply(m_obj, m_func, m_tuple);
	}

private:
	T* m_obj;
	FuncType	m_func;
	std::tuple<Args...> m_tuple;
};

using JobRef = shared_ptr<IJob>;

class JobQueue
{
private:
	USE_LOCK;
	queue<JobRef> m_jobs;

public:
	void Push(JobRef _job)
	{
		WRITE_LOCK;
		m_jobs.push(_job);
	}

	JobRef Pop()
	{
		WRITE_LOCK;
		if (m_jobs.empty())
			return nullptr;

		JobRef ret = m_jobs.front();
		m_jobs.pop();
		return ret;
	}
};