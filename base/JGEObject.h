#ifndef __JGE_OBJECT_H__
#define __JGE_OBJECT_H__

class JGEObject
{
public:
	JGEObject();
	virtual ~JGEObject();

	void setName(const char* lpName);
	inline const char* getName() const { return m_lpName; }

	inline void setExtraData(void* lpData) { m_lpExtraData = lpData; }
	inline void* getExtraData() const { return m_lpExtraData; }

private:
	JGEObject(const JGEObject& value);

	char* m_lpName;
	void* m_lpExtraData;
};

#endif