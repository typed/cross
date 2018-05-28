#ifndef cross_common_singleton_h
#define cross_common_singleton_h

#define DEF_CSINGLETON(T) \
	static void CreateSingleton();\
	static void DestroySingleton();\
	static T& GetSingleton();\
	static T* GetSingletonPtr();\
	static T& GetSingletonAuto();\
	static T* GetSingletonAutoPtr();\
	static T* s_pSingleton;

#define IMP_CSINGLETON(T) \
	T* T::s_pSingleton = 0;\
	void T::CreateSingleton() {s_pSingleton = new T();}\
	void T::DestroySingleton() {delete s_pSingleton; s_pSingleton = 0;}\
	T& T::GetSingleton() {return *s_pSingleton;}\
	T* T::GetSingletonPtr() {return s_pSingleton;}\
	T& T::GetSingletonAuto() {if (s_pSingleton == nullptr) CreateSingleton(); return *s_pSingleton;}\
	T* T::GetSingletonAutoPtr() {if (s_pSingleton == nullptr) CreateSingleton(); return s_pSingleton;}

#define DEF_ISINGLETON(Ti) \
	static void CreateSingleton();\
	static void DestroySingleton();\
	static Ti& GetSingleton();\
	static Ti* GetSingletonPtr();\
	static Ti& GetSingletonAuto();\
	static Ti* GetSingletonAutoPtr();\
	static Ti* s_pSingleton;

#define IMP_ISINGLETON(Ti, Tc) \
	Ti* Ti::s_pSingleton = 0;\
	void Ti::CreateSingleton() {s_pSingleton = new Tc();}\
	void Ti::DestroySingleton() {Tc* p = (Tc*)s_pSingleton; delete p; s_pSingleton = 0;}\
	Ti& Ti::GetSingleton() {return *s_pSingleton;}\
	Ti* Ti::GetSingletonPtr() {return s_pSingleton;}\
	Ti& Ti::GetSingletonAuto() {if (s_pSingleton == nullptr) CreateSingleton(); return *s_pSingleton;}\
	Ti* Ti::GetSingletonAutoPtr() {if (s_pSingleton == nullptr) CreateSingleton(); return s_pSingleton;}

#define DC_INSTANCE(a) static a& Instance();
#define IM_INSTANCE(a) a& a::Instance() {static a _m; return _m;};

#endif
