#ifndef clistenable_h
#define clistenable_h

#include "CommonDef.h"
#include "CrossSTL.h"
#include <vector>

/**
 *	可监听的模板类
 */
template <typename T>
class cListenable
{
public:
	void RegisterListener(T *pListener) {if (pListener) cm::VectorInsertOnly(m_Listeners, pListener);}
	void UnregisterListener(T *pListener) {cm::VectorRemove(m_Listeners, pListener);}
	void UnregisterAllListener() {m_Listeners.clear();}
	std::vector<T*>& GetListeners() {return m_Listeners;}
private:
	std::vector<T*> m_Listeners;
};

#define OnFuncListener(T,func) \
	{ std::vector<T*> tmp = cListenable<T>::GetListeners(); \
	while(!tmp.empty()) { T* p = (T*)tmp.back(); tmp.pop_back(); p->func; } }

#define OnFuncListener2(T,func) {\
		std::vector<T*> tmp = cListenable<T>::GetListeners(); \
		if (!tmp.empty()) {\
			T* p = (T*)tmp.back();\
			tmp.pop_back();\
			return p->func;\
		} else\
			return std::string();\
	}

#define RegListener(T, pListener) cListenable<T>::RegisterListener(pListener);
#define UnregAllListener(T) cListenable<T>::UnregisterAllListener();

#endif
