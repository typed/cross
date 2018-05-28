#ifndef _cnetmapping_h_
#define _cnetmapping_h_

#include "../../inc/CommonDef.h"
#include "../../inc/iNet.h"
#include <string>
#include <map>

namespace cm
{

class cNetMapping
{
public:
	struct stIPPort
	{
		std::string	ip;
		ui16		port;
	};
	cNetMapping();
	void AddressMapping(const std::string& ip, ui16 port, std::string& ipMapped, ui16& portMapped);
	void Use(bool b) {m_bUse = b;}
	static cNetMapping& GetInstance();
private:
	std::map<std::string, stIPPort>	m_mapAddressMapped;
	bool							m_bUse;
};

}

#endif