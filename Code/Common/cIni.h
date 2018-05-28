#ifndef cross_common_cini_h
#define cross_common_cini_h

#include "Common.h"
#include <string>
#include <sstream>
#include <fstream>
#include <map>

namespace cross
{

typedef std::map<std::string, std::string> iniMap;

class cIni
{
public:

	void SetFile(const std::string& szFile)
	{
		m_szFile = szFile;
	}

	bool Load()
	{
		m_map.clear();
		std::ifstream f(m_szFile);
		if (f.is_open()) {
			while (true) {
				std::string sline;
				if (!std::getline(f, sline))
					break;
				i32 of = sline.find("=");
				if (of != std::string::npos) {
					std::string skey = sline.substr(0, of);
					std::string svalue = sline.substr(of+1);
					m_map[skey] = svalue;
				}
			}
			return true;
		}
		return false;
	}

	bool Save()
	{
		std::ofstream f(m_szFile);
		if (!f.is_open())
			return false;
		for (iniMap::iterator it = m_map.begin(); it != m_map.end(); it++)
			f << it->first << "=" << it->second << std::endl;
		return true;
	}

	template <class T> bool GetValue(const std::string& szKey, T& v, T vDefault = T()) const
	{
		iniMap::const_iterator it = m_map.find(szKey);
		if (it == m_map.end()) {
			v = vDefault;
			return false;
		}
		std::istringstream myString(it->second);
		myString >> v;
		v = it->second;
		return true;
	}

	template <class T> bool SetValue(const std::string& szKey, const T& value)
	{
		std::string sValue;
		std::ostringstream myString;
		myString << value;
		sValue = myString.str();
		if (sValue.empty())
			return false;
		m_map[szKey] = sValue;
		return true;
	}

private:
	iniMap			m_map;
	std::string		m_szFile;
};

}
#endif