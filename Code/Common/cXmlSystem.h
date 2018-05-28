#ifndef cxmlsystem_h
#define cxmlsystem_h

#include "../../inc/iXmlSystem.h"
#include "tinyxml.h"
#include "../../inc/CrossString.h"
#include "../../inc/CrossSTL.h"
#include "../../inc/iStream_DS.h"
#include <string>
#include <map>
#include <vector>

namespace cross
{

typedef std::map<std::string, std::string> hmAttributes;
typedef hmAttributes::iterator itAttributes;

class cXmlAttributes : 
	public iXmlAttributes
{
public:
	cXmlAttributes() {}
	~cXmlAttributes() {}
	void Add(cpstr attr, cpstr value) {
		m_hmAttributes[attr] = value;
	}
	void Remove(cpstr attr) {
		m_hmAttributes.erase(attr);
	}
	cpstr GetValue(cpstr attr) const {
		return Find(attr);
	}
	bool GetValueAsString(cpstr attr, cpstr& r, cpstr def = "") const {
		cpstr p = Find(attr);
		if (p == 0) {
			r = def;
			return false;
		}
		r = p;
		return true;
	}
	bool GetValueAsBool(cpstr attr, bool& r, bool def = false) const {
		cpstr p = Find(attr);
		if (p == 0) {
			r = def;
			return false;
		}
		std::string val = p;
		if (val == "false" || val == "0")
			r = false;
		else
			r = true;
		return true;
	}
	bool GetValueAsInteger(cpstr attr, i32& r, i32 def = 0) const {
		cpstr p = Find(attr);
		if (p == 0) {
			r = def;
			return false;
		}
		r = CrossString::_10ToInt(p);
		return true;
	}
	bool GetValueAsFloat(cpstr attr, f32& r, f32 def = 0.0f) const {
		cpstr p = Find(attr);
		if (p == 0) {
			r = def;
			return false;
		}
		r = CrossString::_10ToFloat(p);
		return true;
	}
	bool First() {
		if (m_hmAttributes.empty())
			return false;
		m_itAttributes = m_hmAttributes.begin();
		return true;
	}
	bool Next() {
		if (m_itAttributes == m_hmAttributes.end())
			return false;
		m_itAttributes++;
		if (m_itAttributes == m_hmAttributes.end())
			return false;
		return true;
	}
	cpstr GetCurValue() {
		if (m_itAttributes == m_hmAttributes.end())
			return 0;
		return m_itAttributes->second.c_str();
	}
	cpstr GetCurName() {
		if (m_itAttributes == m_hmAttributes.end())
			return 0;
		return m_itAttributes->first.c_str();
	}
	cpstr Find(cpstr attr) const {
		hmAttributes::const_iterator p = m_hmAttributes.find(attr);
		if (p == m_hmAttributes.end())
			return 0;
		return p->second.c_str();
	}
	hmAttributes	m_hmAttributes;
	itAttributes	m_itAttributes;
};

struct stXmlCache
{
	bool				bStart;
	std::string			element;
	cXmlAttributes	attr;
};
typedef std::vector<stXmlCache> aXmlCache;

class cXmlReader : 
	public iXmlReader
{
public:

	cXmlReader();
	~cXmlReader();

	void Release();

	bool Load(cpstr file, iXmlReaderHandler& handle);
	bool LoadString(cpstr buf, iXmlReaderHandler& handle);
	void Unload();

	void ProcessElement(const TiXmlElement* element);

	iXmlReaderHandler*	m_pHandler;

};

}
#endif
