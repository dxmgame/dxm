#pragma once

#include <assert.h>

//! ���͵��� ======== ��Ҫ���ʱ����ã���ʼ����
#define EAGER_SINGLETON_H(singleton) static singleton* GetInstancePtr(){ static singleton m_nSelf; return &m_nSelf; }

//! ���͵��� ======== 
#define LAZY_SINGLETON_H(singleton)\
public:\
	static singleton* Init()			{ if(NULL == s_pInstance) s_pInstance = new singleton;return s_pInstance; }\
	static void		  Destroy()			{ if(s_pInstance) { delete s_pInstance; s_pInstance = NULL; } }\
	static singleton* GetInstancePtr()	{ return s_pInstance; }\
	static singleton* Shared()			{ return Init(); }\
private:\
	static singleton* s_pInstance;\


#define LAZY_SINGLETON_CPP(singleton) singleton *singleton::s_pInstance = NULL;

//! ģ�嵥�� ======== 
// ==================Ascent�е�ģ�嵥����===================
/* ���˸о�ȱ��:
 * ȱ�㣺
 * ���캯����Ϊ˽�к�������¶����࣬��ι�������assert( s_pSingleton == 0 );
 * �ŵ㣺
 *    1.�̳м��ٴ����� 
 *    2.���캯����Ϊ˽�е��ŵ㣺��ʼ�����Զ�����
 *         �ɼ���(����ȫ�ֱ���createFileSingleton�����ù���������)
 *         �ɻ���(��ʼ������initialiseSingleton�����new�����delete����
 *                �ص�,ֻ�ܹ���һ������,  ����ڶ���new�ͳ���assert( s_pSingleton == 0 ))
*/

// ���ͺ��Ҫnew �� delete����
#define InitialiseSingleton( type ) template <> type * Singleton < type > :: s_pSingleton = 0
// ���ͺ���ùܸö���ĳ�ʼ�����ͷ�
#define CreateSingleton( type ) InitialiseSingleton( type ); type the##type

#ifndef NULL
#define NULL 0
#endif

template < class type > 
class   Singleton {
public:
	Singleton( ) {
		// �쳣��׽��s_pSingleton��Ϊ��
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
	// �쳣��׽��s_pSingletonΪ��
	assert( s_pSingleton ); 
	return *s_pSingleton; 
}
