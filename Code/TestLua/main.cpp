
#include "TestLuaBridge.h"
#include "TestToLuapp.h"
#include "TestLuaStack.h"
#include "Common.h"
#include <iostream>

using namespace cross;

int main(int argc, char const *argv[])
{
    InitCommon();
	//TestLuaBridge(argc, argv);
	TestToLuapp(argc, argv);
	//TestLuaStack(argc, argv);
	ReleaseCommon();
    return 0;
}