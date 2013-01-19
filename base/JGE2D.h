#ifndef __JGE_2D_H__
#define __JGE_2D_H__

#include <JGEInput.h>
#include <JGE3D.h>
#include "jgeUtil.h"

class JGE2D
{
public:
	JGE2D();
	~JGE2D();

	bool init(HINSTANCE hInstance, JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		INT windowX = 0, INT windowY = 0, uint windowWidth = 800, uint windowHeight = 600, bool windowd = TRUE);
};

#endif