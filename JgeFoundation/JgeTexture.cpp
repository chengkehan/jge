#include "JgeStdafx.h"
#include "JgeTexture.h"

jge::Texture2D::Texture2D(Jge3D* lpJge3D):
	m_filter(jge::TextureFilter::TEXTURE_FILTER_NONE), m_width(0), m_height(0), m_lpJge3D(lpJge3D)
{
	// Do nothing
}

jge::Texture2D::Texture2D(Jge3D* lpJge3D, uint width, uint height, TextureFilter filter):
	m_lpJge3D(lpJge3D), m_width(width), m_height(height), m_filter(filter)
{
	// Create Texture
}

jge::Texture2D::~Texture2D()
{
	release();
}

void jge::Texture2D::release()
{
	m_filter = jge::TextureFilter::TEXTURE_FILTER_NONE;
	m_width = 0;
	m_height = 0;
	m_lpJge3D = null;
}