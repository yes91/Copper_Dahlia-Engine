#pragma once

#include <string>
#include <regex>
#include <unordered_map>
#include <iostream>

template<typename T> class TypeId
{
public:
	static std::string qualified_name()
	{
#if defined (_MSC_VER)
		return qualifiedName(__FUNCTION__);
#elif defined(__GNUC__)
		return qualifiedName(__PRETTY_FUNCTION__);
#else
		return qualifiedName(typeid(T).name());
#endif
	}

	static std::string name()
	{
#if defined (_MSC_VER)
		return cleanName(qualifiedName(__FUNCTION__));
#elif defined(__GNUC__)
		return cleanName(qualifiedName(__PRETTY_FUNCTION__));
#else
		return cleanName(qualifiedName(typeid(T).name()));
#endif
	}

	static unsigned int hash_code()
	{
#if defined (_MSC_VER)
		return GenerateTypeId(__FUNCTION__);
#elif defined(__GNUC__)
		return GenerateTypeId(__PRETTY_FUNCTION__);
#else
		return GenerateTypeId(typeid(T).name());
#endif
	}
private:
	TypeId();

	static std::string cleanName(std::string c)
	{
		size_t colons = c.rfind("::");
		if (colons == std::string::npos)
			return c;

		return c.substr(colons + 2, c.length());
	}

	static std::string qualifiedName(std::string s)
	{
		std::string name;

		std::regex expr("<(class|struct) (.*)>");
		std::smatch match; 
		if(std::regex_search(s, match, expr))
		{
			name = match[2].str();
		}
		else
		{
			std::regex typeid_expr("(class|struct) (.*)");
			std::smatch typeid_match;
			if(std::regex_search(s, typeid_match, typeid_expr))
			{
				name = typeid_match[2].str();
			}
		}

		return name;
	}

	static unsigned int GenerateTypeId(const char* s)
	{
		typedef std::unordered_map<const char*, unsigned int> id_cache_type;

		static id_cache_type idCache;

		static std::hash<const char*> hash_func;

		id_cache_type::const_iterator it = idCache.find(s);
		
		if(it != idCache.end())
			return it->second;

		unsigned int hash = hash_func(s);

		idCache[s] = hash;

		return hash;
	}
};