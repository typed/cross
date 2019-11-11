#include <iostream>

using namespace std;

class test1_A
{
public:
    test1_A() {
        cout << "construct test1_A()" << endl;
    }
    ~test1_A() {
        cout << "destruct test1_A()" << endl;
    }
    string m_str;
    int m_num;
};
void test1_do(const char* pstr)
{
    cout << pstr << endl;
}
test1_A get_test1_A() {
    test1_A a;
    a.m_str = "AAAAAA";
    a.m_num = 100;
    return a;
}
void test1()
{
    //pstr 有可能野指针，test1_A::get_test1_A()返回的对象提前析构了
    const char* pstr = get_test1_A().m_str.c_str();
    cout << pstr << " addr " << static_cast<const void *>(pstr) << endl;
    test1_do(get_test1_A().m_str.c_str());
    int num = get_test1_A().m_num;
    cout << num << endl;
}

int main(int argc, char const *argv[])
{
    cout << "test common." << endl;
    test1();
    return 0;
}