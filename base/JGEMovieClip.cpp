#include "JGEMovieClip.h"

JGEMovieClipFrame::JGEMovieClipFrame()
{
	m_lpTexture = null;
	m_refX = 0.0f; m_refY = 0.0f;
}

JGEMovieClipFrame::~JGEMovieClipFrame()
{
	m_lpTexture = null;
	m_refX = 0.0f; m_refY = 0.0f;
}

JGEMovieClip::JGEMovieClip(IDirect3DDevice9* lpd3dd):JGEDisplayObject(lpd3dd)
{
	m_lpFrames = null;
	m_loopFrame = false;
	m_frameIndex = 0;
	m_numFrames = 0;
}

JGEMovieClip::~JGEMovieClip()
{
	destroyFrames();
}

void JGEMovieClip::setFrames(const JGEMovieClipFrame* lpFrames, uint numFrames)
{
	destroyFrames();
	
	if(lpFrames == null || numFrames <= 0)
	{
		return;
	}

	jgeNewArray(m_lpFrames, JGEMovieClipFrame, numFrames);
	jgeMemCpy(lpFrames, m_lpFrames, sizeof(JGEMovieClipFrame) * numFrames);
	m_numFrames = numFrames;
}

bool JGEMovieClip::gotoFrame(int frameIndex)
{
	if(m_numFrames == 0)
	{
		return false;
	}

	if(frameIndex >= m_numFrames || frameIndex < 0)
	{
		if(m_loopFrame)
		{
			int frame = frameIndex % m_numFrames;
			if(frameIndex < 0)
			{
				frameIndex = m_numFrames + frame;
			}
			else
			{
				frameIndex = frame;
			}
		}
		else
		{
			return false;
		}
	}

	m_frameIndex = frameIndex;
	JGEMovieClipFrame* lpFrame = &m_lpFrames[frameIndex];
	setTexture(lpFrame->m_lpTexture);
	setRefX(lpFrame->m_refX);
	setRefY(lpFrame->m_refY);

	return true;
}

void JGEMovieClip::destroyFrames()
{
	jgeDelete(m_lpFrames);
	m_frameIndex = 0;
	m_numFrames = 0;
	setTexture(null);
	setRefX(0.0f);
	setRefY(0.0f);
}

JGEMovieClipData::JGEMovieClipData()
{
	m_lpFrames = null;
	m_numFrames = 0;
}

JGEMovieClipData::~JGEMovieClipData()
{
	jgeDelete(m_lpFrames);
	m_numFrames = 0;
}

bool JGEMovieClipData::loadDataFromFile(const char* lpPath, const JGEMovieClipFrame** lpFrames, uint* lpNumFrames)
{
	if(lpPath == null)
	{
		return false;
	}

	return false;
}