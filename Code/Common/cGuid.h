#ifndef cguid_h
#define cguid_h

#include "CommonDef.h"
#include <string>
#include <string.h>

namespace cross
{

class COMMON_API cGuid
{
public:

	cGuid();

	cGuid(ui32 data1, ui32 data2, ui32 data3, ui32 data4);

	cGuid(const cGuid& guid);

	cGuid& operator = (const cGuid& guid);

	bool operator == (const cGuid& guid) const;

	bool operator != (const cGuid& guid) const {return !operator == (guid);}

	bool operator == (cpstr sguid) const;

	bool operator != (cpstr sguid) const {return !operator == (sguid);}

	void SetString(std::string);
	std::string GetString() const;

	ui32 ToUint() const;

	operator ui32 () const;

	bool operator < (const cGuid& guid) const;

	ui32 operator [] (ui32 i) const {return data[i];}
	ui32& operator [] (ui32 i) {return data[i];}

	void Set0();

	bool Is0();

protected:

	ui32 data[4];

};

COMMON_API bool operator == (ui32 v, const cGuid& guid);

COMMON_API bool operator != (ui32 v, const cGuid& guid);

COMMON_API std::string NewGUID();

}

#endif
