#ifndef __JGE_CALLBACK_H__
#define __JGE_CALLBACK_H__

#include "jgeUtil.h"

enum JGECallbackType
{
	JGECallbackType_This, JGECallbackType_Std
};

template<class Param>
class JGEAbstractCallback
{
public:
	virtual void invoke(Param* lpParam = null) = 0;
	virtual bool equal(JGEAbstractCallback<Param>* lpTarget) = 0;
	inline enum JGECallbackType getType() { return m_type; };

protected:
	enum JGECallbackType m_type;
};

template<class This, class Param>
class JGECallbackThis : public JGEAbstractCallback<Param>
{
public:
	JGECallbackThis() { m_lpThis = null; m_lpCallback = null; m_type = JGECallbackType_This; }
	JGECallbackThis(const JGECallbackThis& value) { m_lpThis = value.m_lpThis; m_lpCallback = value.m_lpCallback; m_type = JGECallbackType_This; }
	JGECallbackThis(This* lpThis, void (This::*lpCallback)(Param*)){ m_lpThis = lpThis; m_lpCallback = lpCallback; m_type = JGECallbackType_This; }
	~JGECallbackThis() { m_lpThis = null; m_lpCallback = null; }
	
	virtual void invoke(Param* lpParam = null) { (m_lpThis->*m_lpCallback)(lpParam); }
	virtual bool equal(JGEAbstractCallback<Param>* lpTarget)
	{
		if(lpTarget == null || lpTarget->getType() != m_type)
		{
			return false;
		}
		else
		{
			JGECallbackThis<This, Param>* lpCallback = (JGECallbackThis<This, Param>*)lpTarget;
			return lpCallback->m_lpThis == m_lpThis && lpCallback->m_lpCallback == m_lpCallback;
		}
	}
	JGECallbackThis<This, Param>* clone(JGECallbackThis<This, Param>* lpDest) 
	{ 
		if(lpDest == null) 
		{ 
			return false; 
		}
		else
		{
			lpDest->m_lpThis = m_lpThis;
			lpDest->m_lpCallback = m_lpCallback;
			return lpDest;
		}
	}

private:
	This* m_lpThis;
	void (This::*m_lpCallback)(Param*);
};

template<class This, class Param>
inline JGECallbackThis<This, Param> jgeCallbackThis(This* lpThis, void (This::*lpCallback)(Param*))
{
	return JGECallbackThis<This, Param>(lpThis, lpCallback);
}

template<class Param>
class JGECallbackStd : public JGEAbstractCallback<Param>
{
public:
	JGECallbackStd() { m_lpCallback = null; m_type = JGECallbackType_Std; }
	JGECallbackStd(const JGECallbackStd& value) { m_lpCallback = value.m_lpCallback; m_type = JGECallbackType_Std; }
	JGECallbackStd(void (*lpCallback)(Param*)) { m_lpCallback = lpCallback; m_type = JGECallbackType_Std; }
	~JGECallbackStd() { m_lpCallback = null; }

	virtual void invoke(Param* lpParam = null) { m_lpCallback(lpParam); }
	virtual bool equal(JGEAbstractCallback<Param>* lpTarget)
	{
		if(lpTarget == null || lpTarget->getType() != m_type)
		{
			return false;
		}
		else
		{
			JGECallbackStd<Param>* lpCallback = (JGECallbackStd<Param>*)lpTarget;
			return lpCallback->m_lpCallback == m_lpCallback;
		}
	}
	JGECallbackStd<Param>* clone(JGECallbackStd<Param>* lpDest)
	{
		if(lpDest == null)
		{
			return null;
		}
		else
		{
			lpDest->m_lpCallback = m_lpCallback;
			return lpDest;
		}
	}

private:
	void (*m_lpCallback)(Param*);
};

template<class Param>
inline JGECallbackStd<Param> jgeCallbackStd(void (*lpCallback)(Param*))
{
	return JGECallbackStd<Param>(lpCallback);
}

template<class Param>
inline void jgeCallbackRelease(JGEAbstractCallback<Param>* lpCallback)
{
	jgeDelete(lpCallback);
}

#endif