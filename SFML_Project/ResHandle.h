#pragma once

#include "ResCache.h"
#include "EngineStd.h"

template<class R>
class ResHandle
{
	template<class T>
	friend class ResCache;
protected:
	/*The member m_name and the get() const and getWrite() accessors are the only reasons ResHandle exists.
	Otherwise, this class would be excahnged for a simple shared_ptr to the the resource.*/
	std::string m_name; 
	R* m_pRes;

public:

	ResHandle(std::string name, R* p_res)
		:m_name(name)
	{
		m_pRes = p_res;
	}

	~ResHandle()
	{
		std::cout << "[ResourceFreed] " << m_name << std::endl; 
		SAFE_DELETE(m_pRes);
	}

	R& get() const { return *m_pRes; }
	R& getWrite() { return *m_pRes; }
};

template<class R> 
struct Resource
{
	typedef std::shared_ptr<ResHandle<R>> type;
};
