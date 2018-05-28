#include "cNetMapping.h"
#include "../../inc/CrossString.h"
#include <fstream>

using namespace std;

namespace cm
{

static const cpstr c_sMappingFile = "proxy.txt";

cNetMapping::cNetMapping()
{
	m_bUse = false;
	std::ifstream ifs(c_sMappingFile);
	if (!ifs.is_open())
		return;
	while(ifs.good()) {
		std::string sName, sIP;
		ui16 port;
		if (ifs >> sName >> sIP >> port) {
			stIPPort& ipport = m_mapAddressMapped[sName];
			ipport.ip = sIP;
			ipport.port = port;
		}
	}
}

void cNetMapping::AddressMapping(const std::string& ip, ui16 port, std::string& ipMapped, ui16& portMapped)
{
	string ipIn = DomainToIP(ip.c_str());
	if (!m_bUse) {
		ipMapped = ipIn;
		portMapped = port;
		return;
	}
	string name = CrossString::Format("%s_%d", ipIn.c_str(), port);
	map<string, stIPPort>::iterator it = m_mapAddressMapped.find(name);
	if (it == m_mapAddressMapped.end()) {
		ipMapped = ipIn;
		portMapped = port;
		return;
	}
	else {
		ipMapped = it->second.ip;
		portMapped = it->second.port;
	}
}

cNetMapping& cNetMapping::GetInstance()
{
	static cNetMapping s_map;
	return s_map;
}

}