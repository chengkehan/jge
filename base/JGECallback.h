#ifndef __JGE_CALLBACK_H__
#define __JGE_CALLBACK_H__

#include "jgeUtil.h"

template<class Param>
class JGEAbstractCallback
{
public:
	virtual void operator()(Param* lpParam = null) = 0;
};

template<class This, class Param>
class JGECallbackThis : public JGEAbstractCallback<Param>
{
public:
	JGECallbackThis(const JGECallbackThis& value) { m_lpThis = value.m_lpThis; m_lpCallback = value.m_lpCallback; }
	JGECallbackThis(This* lpThis, void (This::*lpCallback)(Param*)){ m_lpThis = lpThis; m_lpCallback = lpCallback; }
	~JGECallbackThis() { m_lpThis = null; m_lpCallback = null; }
	
	virtual void operator()(Param* lpParam = null) { (m_lpThis->*m_lpCallback)(lpParam); }

private:
	JGECallbackThis();

	This* m_lpThis;
	void (This::*m_lpCallback)(Param*);
};

template<class This, class Param>
inline JGEAbstractCallback<Param>* jgeCallbackThis(This* lpThis, void (This::*lpCallback)(Param*))
{
	return (new JGECallbackThis<This, Param>(lpThis, lpCallback));
}

template<class Param>
class JGECallbackStd : public JGEAbstractCallback<Param>
{
public:
	JGECallbackStd(const JGECallbackStd& value) { m_lpCallback = value.m_lpCallback; }
	JGECallbackStd(void (*lpCallback)(Param*)) { m_lpCallback = lpCallback; }
	~JGECallbackStd() { m_lpCallback = null; }

	virtual void operator()(Param* lpParam = null) { m_lpCallback(lpParam); }

private:
	void (*m_lpCallback)(Param*);
};

template<class Param>
inline JGEAbstractCallback<Param>* jgeCallbackStd(void (*lpCallback)(Param*))
{
	return (new JGECallbackStd<Param>(lpCallback));
}

template<class Param>
inline void jgeCallbackRelease(JGEAbstractCallback<Param>* lpCallback)
{
	jgeDelete(lpCallback);
}

#endif