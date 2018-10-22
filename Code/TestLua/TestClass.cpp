#include "TestClass.h"
#include "Common.h"
#include "iLog.h"

using namespace std;
using namespace cross;

lua_State* g_lua_State = nullptr;

RefLuaBind::RefLuaBind()
{
	static int s_uObjectCount = 0;
	_luaID = 0;
	_ID = ++s_uObjectCount;
}

RefLuaBind::~RefLuaBind()
{
	if (g_lua_State) {
		toluafix_remove_ccobject_by_refid(g_lua_State, _luaID);
	}
}

Node::Node()
{
    m_pParent = nullptr;
    m_iTag = 0;
}

Node::~Node()
{
    RemoveAllChild();
    if (g_lua_State) {
        toluafix_remove_usertable_by_refid(g_lua_State, _luaID);
    }
}

Node* Node::New()
{
    return new Node();
}

void Node::SetName(const string& name)
{
    m_sName = name;
}

const string& Node::GetName() const
{
	return m_sName;
}

void Node::SetTag(int tag)
{
    m_iTag = tag;
}

int Node::GetTag() const
{
    return m_iTag;
}

bool Node::AddChild(Node* pNode)
{
    if (pNode == nullptr || pNode->m_pParent != nullptr)
        return false;
    for (const auto& nd : m_vChildren) {
        if (nd == pNode) {
            return false;
        }
    }
    SafeAddRef(pNode);
    pNode->m_pParent = this;
    m_vChildren.push_back(pNode);
    return true;
}

bool Node::RemoveChild(Node* pNode)
{
    for (auto it = m_vChildren.begin(); it != m_vChildren.end(); it++) {
        if (pNode == *it) {
            m_vChildren.erase(it);
            SafeRelease(pNode);
            return true;
        }
    }
    return false;
}

void Node::RemoveAllChild()
{
    for (auto nd : m_vChildren) {
        SafeRelease(nd);
    }
    m_vChildren.clear();
}

void Node::RemoveFromParent()
{
    if (m_pParent == nullptr)
        return;
    RemoveAllChild();
    m_pParent->RemoveChild(this);
}

Node* Node::FindChildByName(const string& name)
{
	for (auto nd : m_vChildren) {
		if (nd->GetName() == name) {
			return nd;
		}
	}
	return nullptr;
}
	
void Node::Trace()
{
    LInfo << GetClassType() << LEnd;
    LInfo << "  m_sName=" << m_sName << " m_iTag=" << m_iTag << LEnd;
    for (vector<Node*>::iterator it = m_vChildren.begin(); it != m_vChildren.end(); it++) {
        (*it)->Trace();
    }
}

Char::Char()
{
    m_iAge = 0;
}

Char* Char::New()
{
    return new Char();
}
void Char::SetAge(int age)
{
    m_iAge = age;
}
int Char::GetAge() const
{
    return m_iAge;
}

void Char::Trace()
{
    __super::Trace();
    LInfo << "  m_iAge=" << m_iAge << LEnd;
}

Player::Player()
{
    m_iControl = 0;
}

Player* Player::New()
{
    return new Player();
}

void Player::SetControl(int c)
{
    m_iControl = c;
}

int Player::GetControl() const {
    return m_iControl;
}

void Player::Trace()
{
    __super::Trace();
    LInfo << "  m_iControl=" << m_iControl << LEnd;
}

Node* GetRootNode()
{
    static Node s_rootNode;
	return &s_rootNode;
}