#ifndef __JGE_H__
#define __JGE_H__

#include "JGE3D.h"
#include "JGE2D.h"
#include "JGETexture.h"
#include "JGETextureManager.h"
#include "JGEDisplayObjectContainer.h"
#include "jgeUtil.h"

namespace jge
{
	extern JGE2D* lpJge;

	inline bool init(HINSTANCE hInstance, JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		int windowX = 0, int windowY = 0, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true)
	{
		lpJge = JGE2D::getInstance();
		return lpJge->init(hInstance, setupCallback, releaseCallback, frameCallback, windowX, windowY, windowWidth, windowHeight, windowd);
	}

	inline void run()
	{
		JGE3D::getInstance()->run();
	}

	inline JGETexture* loadTexture(int id, const char* lpPath)
	{
		return JGETextureManager::getInstance()->loadFileTexture(id, lpPath, JGE3D::getInstance()->getDirect3DDevice());
	}

	inline JGETexture* getTexture(int id)
	{
		return JGETextureManager::getInstance()->getTexture(id);
	}

	inline JGEDisplayObjectContainer* getStage()
	{
		return JGE2D::getInstance()->getStage();
	}
}

#endif