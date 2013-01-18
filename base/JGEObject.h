#ifndef __JGE_OBJECT_H__
#define __JGE_OBJECT_H__

class JGEObject
{
public:
	JGEObject();
	virtual ~JGEObject();

	void setName(const char* lpName);
	const char* getName() const;

	void setExtraData(void* lpData);
	void* getExtraData() const;

private:
	JGEObject(const JGEObject& value);

	char* m_lpName;
	void* m_lpExtraData;
};

#endif