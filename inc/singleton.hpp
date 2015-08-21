#pragma once

#include <assert.h>

//! 急型单件 ======== 需要最初时候调用，初始化的
#define EAGER_SINGLETON_H(singleton) static singleton* GetInstancePtr(){ static singleton m_nSelf; return &m_nSelf; }

//! 缓型单件 ======== 
#define LAZY_SINGLETON_H(singleton)\
public:\
	static singleton* Init()			{ if(NULL == s_pInstance) s_pInstance = new singleton;return s_pInstance; }\
	static void		  Destroy()			{ if(s_pInstance) { delete s_pInstance; s_pInstance = NULL; } }\
	static singleton* GetInstancePtr()	{ return s_pInstance; }\
	static singleton* Shared()			{ return Init(); }\
private:\
	static singleton* s_pInstance;\


#define LAZY_SINGLETON_CPP(singleton) singleton *singleton::s_pInstance = NULL;

//! 模板单件 ======== 
// ==================Ascent中的模板单件法===================
/* 个人感觉缺点:
 * 缺点：
 * 构造函数不为私有函数，暴露这个类，多次构造会出现assert( s_pSingleton == 0 );
 * 优点：
 *    1.继承减少代码量 
 *    2.构造函数不为私有的优点：初始化可以多样，
 *         可急型(定义全局变量createFileSingleton，不用管生存周期)
 *         可缓型(初始化变量initialiseSingleton后进行new构造和delete析构
 *                特点,只能构造一个对象,  构造第二次new就出现assert( s_pSingleton == 0 ))
*/

// 缓型宏后要new 和 delete对象
#define InitialiseSingleton( type ) template <> type * Singleton < type > :: s_pSingleton = 0
// 急型宏后不用管该对象的初始化和释放
#define CreateSingleton( type ) InitialiseSingleton( type ); type the##type

#ifndef NULL
#define NULL 0
#endif

template < class type > 
class   Singleton {
public:
	Singleton( ) {
		// 异常捕捉：s_pSingleton不为空
		assert( s_pSingleton == NULL );
		s_pSingleton = static_cast<type *>(this);
	}
	virtual ~Singleton( ) { s_pSingleton = NULL;	}

	static type & GetInstance();

	static type * GetInstancePtr( );

protected:
	static type * s_pSingleton;
};

template < class type >
type * Singleton<type>::GetInstancePtr()
{
	return s_pSingleton; 
}

template < class type >
type & Singleton<type>::GetInstance()
{
	// 异常捕捉：s_pSingleton为空
	assert( s_pSingleton ); 
	return *s_pSingleton; 
}
