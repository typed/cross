#include "ELuna/ELuna.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ELuna;

class Node
{
public:
	Node() {
		m_sName = "Node";
		m_iOrder = 0;
		m_iType = 0;
	}
	virtual void Display() {
		cout << "m_sName=" << m_sName << " m_iType=" << m_iType << " m_iOrder=" << m_iOrder << endl;
	}
	int m_iOrder;
	int m_iType;
	string m_sName;
};

int main(int argc, char const* argv[])
{
    cout << "TestELuna." << endl;
	lua_State* L = ELuna::openLua();

	ELuna::closeLua(L);

    return 0;
}