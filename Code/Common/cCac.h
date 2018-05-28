#include "iCac.h"
#include "iStream_DS.h"
#include "iCompress.h"
#include <map>

typedef std::map<std::string, std::string> mapCac;

namespace cross
{

struct stCacHeader
{
	ui32	name;
	ui32	compress_size;
	ui32	src_size;
	i8		ext[100];
};

class cCac :
	public iCac
{
public:
	cCac();
	~cCac();
	void Release() {delete this;}
	bool LoadFromFile(const std::string&);
	bool Load(const std::string&);
	bool Load(cpvd, ui32);
	std::string Save();
	void Set(std::string vKey, std::string vValue);
	bool Get(std::string vKey, std::string& vValue);
	bool Del(std::string vKey);
	void SetUI32(ui32 vKey, std::string vValue);
	bool GetUI32(ui32 vKey, std::string& vValue);
	bool DelUI32(ui32 vKey);
private:
	mapCac			m_mapCac;
};

}