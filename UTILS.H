#ifndef UTILS_H
#define UTILS_H

//-----------------------------------------------------------------------------
// Includes

#include <iostream.h>

//-----------------------------------------------------------------------------
// macro which returns the number of elements in an array

#undef ELEMENTS
#define ELEMENTS(array) (sizeof(array)/sizeof(array[0]))

//-----------------------------------------------------------------------------
// macro to query a COM interface

#undef QUERY_INTERFACE
#define QUERY_INTERFACE(pUnknown, iid, pInterface) \
pUnknown->QueryInterface(IID_##iid, reinterpret_cast<void**>(&pInterface))

//-----------------------------------------------------------------------------
// macro to conditionally release a COM interface

#undef RELEASE
#define RELEASE(pInterface) if (pInterface != NULL) { pInterface->Release(); pInterface = NULL; }

//-----------------------------------------------------------------------------
// macro to conditionally release (unref()) an OpenInventor node

#undef RELEASE_NODE
#define RELEASE_NODE(pNode) if (pNode != NULL) { pNode->unref(); pNode = NULL; }

//-----------------------------------------------------------------------------
// macro to safely call a COM interface method

#undef STDCOMCALL
#define STDCOMCALL(pInterface, method) \
	(pInterface != NULL ? pInterface->method : E_NOINTERFACE); \
	ASSERT(pInterface != NULL)

//-----------------------------------------------------------------------------
// macro which returns a dereferenced "this" pointer

#undef self
#define self (*this)

//-----------------------------------------------------------------------------
// DEBUG Helper Macros

#ifdef _DEBUG
#  define DBPRINT(msg) afxDump << msg << "\n"
#else
#  define DBPRINT(msg)
#endif

#undef MESSAGE_BOX
#define MESSAGE_BOX(msg) \
{ \
  TRACE msg; TRACE("\n");\
  CString strMsg; \
  strMsg.Format msg; \
  AfxMessageBox(strMsg, MB_OK | MB_ICONEXCLAMATION); \
}

class CTracer
{
public:
	CTracer(LPCTSTR pszTrace) : m_strTrace(pszTrace) { In(); }
	CTracer(CString strTrace) : m_strTrace(strTrace) { In(); }

	void In()      { TRACE("In : %s\n", (LPCTSTR)m_strTrace); }
	void Message() { TRACE("Msg: %s: ", (LPCTSTR)m_strTrace); }
	void Error()   { TRACE("Err: %s: ", (LPCTSTR)m_strTrace); }
	void Out()     { TRACE("Out: %s\n", (LPCTSTR)m_strTrace);	}

	~CTracer() { Out(); }

private:
	CString m_strTrace;
};

#undef TRACE_IN
#undef TRACE_MSG
#undef TRACE_ERR

#ifdef DEBUG_TRACE
#  define TRACE_IN(psz) CTracer _tracer(psz)
#  define TRACE_MSG(msg) _tracer.Message(); TRACE msg; TRACE("\n");
#  define TRACE_ERR(msg) _tracer.Error(); TRACE("in file %s at line %d:\n  ", __FILE__, __LINE__); TRACE msg
#else
#  define TRACE_IN(psz)
#  define TRACE_MSG(msg)
#  define TRACE_ERR(msg) \
MESSAGE_BOX(("Error detected in file %s at line %d.", __FILE__, __LINE__)); \
MESSAGE_BOX(msg)
#endif

//-----------------------------------------------------------------------------
// COM Helper Macros

#define IMPLEMENT_IUNKNOWN_ADDREF(ObjectClass, InterfaceClass) \
STDMETHODIMP_(ULONG) ObjectClass::X##InterfaceClass::AddRef() \
{ \
  METHOD_PROLOGUE_EX_(ObjectClass, InterfaceClass) \
  return pThis->ExternalAddRef(); \
}

#define IMPLEMENT_IUNKNOWN_RELEASE(ObjectClass, InterfaceClass) \
STDMETHODIMP_(ULONG) ObjectClass::X##InterfaceClass::Release(void) \
{ \
  METHOD_PROLOGUE_EX_(ObjectClass, InterfaceClass) \
  return pThis->ExternalRelease(); \
}

#define IMPLEMENT_IUNKNOWN_QUERYINTERFACE(ObjectClass, InterfaceClass) \
STDMETHODIMP ObjectClass::X##InterfaceClass::QueryInterface(REFIID riid, LPVOID* ppVoid) \
{ \
  METHOD_PROLOGUE_EX_(ObjectClass, InterfaceClass) \
  return (HRESULT)pThis->ExternalQueryInterface(&riid, ppVoid); \
}

#define IMPLEMENT_IUNKNOWN(ObjectClass, InterfaceClass) \
  IMPLEMENT_IUNKNOWN_ADDREF(ObjectClass, InterfaceClass) \
  IMPLEMENT_IUNKNOWN_RELEASE(ObjectClass, InterfaceClass) \
  IMPLEMENT_IUNKNOWN_QUERYINTERFACE(ObjectClass, InterfaceClass)

//-----------------------------------------------------------------------------
// Automatic (smart) pointer class template (see p. 293 of "More Effective C++", by
// Scott Meyers.

// explicit keyword not yet supported
#define explicit

// use my version of auto_ptr until compiler provides it
#define auto_ptr CAutoPtr

template<class T> class CAutoPtr
{
public:
	// constructors
	explicit CAutoPtr(T* pT = NULL) : m_pT(pT) { }
#if 0
	template<class U> CAutoPtr(CAutoPtr<U>& rhs) : m_pT(rhs.release()) { }
#else
	CAutoPtr(CAutoPtr<T>& rhs) : m_pT(rhs.release()) { }
#endif

	// destructor
	~CAutoPtr() { delete m_pT; }

	// assignment operator
#if 0
	template<class U>	CAutoPtr<T>& operator=(CAutoPtr<U>& rhs) {
		if (this != rhs) reset(rhs.release());
		return self;
	}
#else
	CAutoPtr<T>& operator=(CAutoPtr<T>& rhs) {
		if (this != &rhs) reset(rhs.release());
		return self;
	}
#endif

	// dereference operator
	T& operator*() const { return *m_pT; }

	// pointer access operator
	T* get() const { return m_pT; }

	// pointer operator
	T* operator->() const { return get(); }

	// release pointer
	T* release() { T* pT = m_pT; m_pT = NULL; return pT; }

	// reset pointer
	void reset(T* pT = NULL) { delete m_pT; m_pT = pT; }

private:
	T* m_pT;
};

#endif