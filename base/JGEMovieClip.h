#ifndef __JGE_MOVIECLIP_H__
#define __JGE_MOVIECLIP_H__

#include "JGEDisplayObject.h"

class JGEMovieClipFrame
{
public:
	JGEMovieClipFrame();
	~JGEMovieClipFrame();

	JGETexture* m_lpTexture;
	float m_refX;
	float m_refY;
};

class JGEMovieClip : public JGEDisplayObject
{
public:
	JGEMovieClip(IDirect3DDevice9* lpd3dd);
	~JGEMovieClip();

	void setFrames(const JGEMovieClipFrame* lpFrames, uint numFrames);
	inline const JGEMovieClipFrame* getFrames() const { return m_lpFrames; }
	inline int getNumFrames() const { return m_numFrames; }
	bool gotoFrame(int frameIndex);
	inline int getCurrentFrameIndex() const { return m_frameIndex; }
	inline void setLoopFrame(bool loop) { m_loopFrame = loop; }
	inline bool getLoopFrame() const { return m_loopFrame; }

private:
	JGEMovieClip();
	JGEMovieClip(const JGEMovieClip& value);

	JGEMovieClipFrame* m_lpFrames;
	int m_numFrames;
	int m_frameIndex;
	bool m_loopFrame;

	void destroyFrames();
};

class JGEMovieClipData
{
public:
	JGEMovieClipData();
	~JGEMovieClipData();

	bool loadDataFromFile(const char* lpPath, const JGEMovieClipFrame** lpFrames = null, uint* lpNumFrames = null);
	inline const JGEMovieClipFrame* getFrames() const { return m_lpFrames; }
	inline const uint getNumFrames() const { return m_numFrames; }

private:
	JGEMovieClipData(const JGEMovieClipData& value);

	JGEMovieClipFrame* m_lpFrames;
	uint m_numFrames;
};

#endif