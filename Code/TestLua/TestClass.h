#pragma once

#include "iReference.h"
#include "tolua_fix.h"
#include <string>

using namespace std;
using namespace cross;

extern lua_State* g_lua_State;

class RefLuaBind : public iReference
{
public:
	RefLuaBind();
	virtual ~RefLuaBind();
	int _ID;
	int _luaID;
};

class Node : public RefLuaBind
{
public:
	Node();
	virtual ~Node();
	static Node* New();
	void SetName(const string& name);
	const string& GetName() const;
	void SetTag(int tag);
	int GetTag() const;
	virtual const char* GetClassType() {
		return "Node";
	}
	virtual bool AddChild(Node* pNode);
	virtual bool RemoveChild(Node* pNode);
	virtual void RemoveAllChild();
	virtual void RemoveFromParent();
	virtual Node* FindChildByName(const string& name);
	virtual void Trace();
protected:
	int m_iTag;
	string m_sName;
	Node* m_pParent;
	vector<Node*> m_vChildren;
};

class Char : public Node
{
public:
	Char();
	static Char* New();
	void SetAge(int age);
	int GetAge() const;
	virtual const char* GetClassType() {
		return "Char";
	}
	virtual void Trace();
protected:
	int m_iAge;
};

class Player : public Char
{
public:
	Player();
	static Player* New();
	void SetControl(int c);
	int GetControl() const;
	virtual const char* GetClassType() {
		return "Player";
	}
	virtual void Trace();
protected:
	int m_iControl;
};

Node* GetRootNode();