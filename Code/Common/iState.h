#ifndef istate_h
#define istate_h

#include "CommonDef.h"

namespace cm
{

enum class eState_Excute_Type
{
	Continue,
	Delete
};

template<typename T>
class iState
{
public:
	virtual ~iState() {}
	virtual i32 GetType() const = 0;
	virtual b8 CanEnter(T*, iState<T>* pre) {return true;}
	virtual void Enter(T*) {}
	virtual void Leave(T*, iState<T>* next) {}
	virtual eState_Excute_Type Excute(T*) {return eState_Excute_Type::Continue;}
};

template<typename T>
class cStateMgr
{
public:
	cStateMgr(T* obj) {
		m_pCurState = nullptr;
		m_pPreState = nullptr;
		m_pObj = obj;
	}
	virtual ~cStateMgr(void) {
		DEL(m_pCurState);
		DEL(m_pPreState);
		m_pObj = nullptr;
	}
	virtual void Excute() {
		if (m_pCurState) {
			if (eState_Excute_Type::Delete == m_pCurState->Excute(m_pObj))
				EnterState(nullptr);
		}
	}
	virtual void EnterState(iState<T>* pState) {
		if (pState && !pState->CanEnter(m_pObj, m_pCurState)) {
			DEL(pState);
			return;
		}
		DEL(m_pPreState);
		m_pPreState = m_pCurState;
		if (m_pCurState)
			m_pCurState->Leave(m_pObj, pState);
		m_pCurState = pState;
		if (m_pCurState)
			m_pCurState->Enter(m_pObj);
	}
	iState<T>* GetPreState() const {return m_pPreState;}
	iState<T>* GetCurState() const {return m_pCurState;}
	void ClearPreState() {DEL(m_pPreState);}
	T* GetObj() const {return m_pObj;}
private:
	iState<T>*		m_pCurState;
	iState<T>*		m_pPreState;
	T*				m_pObj;
};

}

#endif