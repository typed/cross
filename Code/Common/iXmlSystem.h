#ifndef cross_common_ixmlsystem_h
#define cross_common_ixmlsystem_h

#include "Common.h"
#include <string>

namespace cross
{

class iXmlAttributes
{
public:
	virtual bool GetValueAsString(cpstr attr, cpstr&, cpstr def = "") const = 0;
	virtual bool GetValueAsBool(cpstr attr, bool&, bool def = false) const = 0;
	virtual bool GetValueAsInteger(cpstr attr, i32&, i32 def = 0) const = 0;
	virtual bool GetValueAsFloat(cpstr attr, f32&, f32 def = 0.0f) const = 0;
	bool GetValueRef(cpstr attr, std::string& v) const {cpstr s; if (GetValueAsString(attr, s)) { v = s; return true; } return false; }
	bool GetValueRef(cpstr attr, bool& v) const {bool b; if (GetValueAsBool(attr, b)) { v = b; return true; } return false; }
	bool GetValueRef(cpstr attr, i32& v) const {i32 n; if (GetValueAsInteger(attr, n)) { v = n; return true; } return false; }
	bool GetValueRef(cpstr attr, f32& v) const {f32 n; if (GetValueAsFloat(attr, n)) { v = n; return true; } return false; }
	bool GetValueRef(cpstr attr, i64& v) const {i32 n; if (GetValueAsInteger(attr, n)) { v = n; return true; } return false; }
	bool GetValueRef(cpstr attr, f64& v) const {f32 n; if (GetValueAsFloat(attr, n)) { v = n; return true; } return false; }
	virtual bool First() = 0;
	virtual bool Next() = 0;
	virtual cpstr GetCurValue() = 0;
	virtual cpstr GetCurName() = 0;
};

class iXmlReaderHandler
{
public:
	virtual void ElementStart(cpstr element, iXmlAttributes& attributes, cpstr elementTxt) {}
	virtual void ElementEnd(cpstr element) {}
};

class iXmlReader
{
public:
	static iXmlReader* CreateInstance();
	static void test();
	virtual void Release() = 0;
	virtual bool Load(cpstr file, iXmlReaderHandler& handle) = 0;
	virtual bool LoadString(cpstr buf, iXmlReaderHandler& handle) = 0;
	virtual void Unload() = 0;
};

}

#endif
