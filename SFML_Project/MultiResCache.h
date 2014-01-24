#pragma once

#include "EngineStd.h"
#include "ResCache.h"
#include "ThreadInfo.h"
#include "TypeId.h"
#include <mutex>

//Type Erasure
struct CacheHolderBase
{
	virtual void flush() = 0;
	virtual ~CacheHolderBase() {}
};

template <class R>
struct CacheHolder : CacheHolderBase
{
	ResCache<R> cache;
	void flush() { cache.Flush(); }
};

class MultiResCache
{
private:
	std::mutex mutex;

	static MultiResCache* p_instance;

	MultiResCache() {}

	MultiResCache(MultiResCache&);
	void operator=(MultiResCache&);

	~MultiResCache() {}

private:
	typedef std::map<unsigned int, std::unique_ptr<CacheHolderBase>> ResCacheMap;
	typedef ResCacheMap::iterator ResCacheIterator;

private:

	template <class R>
	ResCache<R>* findCache()
	{
		ResCacheIterator itr = m_Caches.find(TypeId<R>::hash_code());
		if (itr == m_Caches.end())
			return nullptr;	
		else
			return &dereference<R>(itr);
	}

	template <class R>
	ResCache<R>& dereference(ResCacheIterator itr)
	{
		return static_cast<CacheHolder<R>&>(*itr->second).cache;
	}

private:
	ResCacheMap m_Caches;

public:

	static MultiResCache& getInstance()
	{
		static MultiResCache theInstance;
        p_instance = &theInstance;

		return *p_instance;
	}

	template<class R>
	shared_ptr< ResHandle < R > > GetHandle(std::string name)
	{
		std::lock_guard<std::mutex> lock(mutex);

		ResCache<R>* cache = findCache<R>();
		if(!cache)
		{
			CacheHolder<R>* holder_concrete = new CacheHolder<R>();
			cache = &holder_concrete->cache;
			std::unique_ptr<CacheHolderBase> holder(holder_concrete);
			// Insert cache to the map
			m_Caches[TypeId<R>::hash_code()] = std::move(holder);	
		}

		auto handle = cache->GetHandle(name);

		glFlush();

		return handle;
	}

	template<class R, typename T>
	shared_ptr< ResHandle < R > > GetHandle(std::string name, T arg2)
	{
		std::lock_guard<std::mutex> lock(mutex);
		ResCache<R>* cache = findCache<R>();
		if(!cache)
		{
			CacheHolder<R>* holder_concrete = new CacheHolder<R>();
			cache = &holder_concrete->cache;
			std::unique_ptr<CacheHolderBase> holder(holder_concrete);
			// Insert cache to the map
			m_Caches[TypeId<R>::hash_code()] = std::move(holder);	
		}

		auto handle = cache->GetHandle(name, arg2);

		glFlush();

		return handle;
	}

	void Flush()
	{
		std::lock_guard<std::mutex> lock(mutex);
		for(auto& cache : m_Caches)
		{
			cache.second->flush();
		}
	}

};