#include "JGEScale9GridDisplayObject.h"
#include "JGERender.h"

JGEScale9GridDisplayObject::JGEScale9GridDisplayObject(IDirect3DDevice9* lpd3dd):JGEDisplayObject(lpd3dd)
{
	m_numRenderRect = 9;
}

JGEScale9GridDisplayObject::~JGEScale9GridDisplayObject()
{
	// Do nothing
}

bool JGEScale9GridDisplayObject::setScale9Grid(const JGERect* lpRect)
{
	if(lpRect == null)
	{
		return false;
	}
	else
	{
		m_scale9Grid = *lpRect;
		return true;
	}
}

JGERect* JGEScale9GridDisplayObject::getBoundsGlobal(JGERect* lpRectResult)
{
	if(lpRectResult == null)
	{
		return null;
	}

	if(m_lpVBData == null || m_lpTexture == null)
	{
		return null;
	}
	else
	{
		lpRectResult->m_left = min(min(min(m_lpVBData[24].x, m_lpVBData[1].x), m_lpVBData[34].x), m_lpVBData[11].x);
		lpRectResult->m_top = min(min(min(m_lpVBData[24].y, m_lpVBData[1].y), m_lpVBData[34].y), m_lpVBData[11].y);
		lpRectResult->m_right = max(max(max(m_lpVBData[24].x, m_lpVBData[1].x), m_lpVBData[34].x), m_lpVBData[11].x);
		lpRectResult->m_bottom = max(max(max(m_lpVBData[24].y, m_lpVBData[1].y), m_lpVBData[34].y), m_lpVBData[11].y);
		return lpRectResult;
	}
}

void JGEScale9GridDisplayObject::render()
{
	if(!shownInDisplayList())
	{
		return;
	}

	// create vb
	if(m_lpVBData == null)
	{
		jgeNewArray(m_lpVBData, JGEDisplayObject::Vertex, 4 * sizeof(JGEDisplayObject::Vertex) * m_numRenderRect);
		for(UINT i = 0; i < m_numRenderRect; ++i)
		{
			UINT index = i * 4;
			m_lpVBData[index].u = 0.0f; m_lpVBData[index].v = 1.0f; m_lpVBData[index].diffuse = 0xFF000000; m_lpVBData[index].x = 0.0f; m_lpVBData[index].y = 0.0f; m_lpVBData[index].rhw = 1.0f; m_lpVBData[index].z = 0.0f;
			++index;
			m_lpVBData[index].u = 0.0f; m_lpVBData[index].v = 0.0f; m_lpVBData[index].diffuse = 0xFF000000; m_lpVBData[index].x = 0.0f; m_lpVBData[index].y = 0.0f; m_lpVBData[index].rhw = 1.0f; m_lpVBData[index].z = 0.0f;
			++index;
			m_lpVBData[index].u = 1.0f; m_lpVBData[index].v = 1.0f; m_lpVBData[index].diffuse = 0xFF000000; m_lpVBData[index].x = 0.0f; m_lpVBData[index].y = 0.0f; m_lpVBData[index].rhw = 1.0f; m_lpVBData[index].z = 0.0f;
			++index;
			m_lpVBData[index].u = 1.0f; m_lpVBData[index].v = 0.0f; m_lpVBData[index].diffuse = 0xFF000000; m_lpVBData[index].x = 0.0f; m_lpVBData[index].y = 0.0f; m_lpVBData[index].rhw = 1.0f; m_lpVBData[index].z = 0.0f;
		}
	}

	// update uv
	// --------------------------
	// |1	  3	|5	  7	|9	  11|
	// |0 (1) 2	|4 (2)6	|8 (3)10|
	// |--------|-------|-------|
	// |13	  15|17	  19|21	  23|
	// |12 (4)14|16(5)18|20(6)22|
	// |--------|-------|-------|
	// |25	  27|29	  31|33	  35|
	// |24(7) 26|28(8)30|32(9)34|
	// --------------------------
	float u = (float)m_lpTexture->getImageInfo()->Width / (float)m_lpTexture->getSurfaceDesc()->Width;
	float v = (float)m_lpTexture->getImageInfo()->Height / (float)m_lpTexture->getSurfaceDesc()->Height;
	float width = (float)m_lpTexture->getImageInfo()->Width;
	float height = (float)m_lpTexture->getImageInfo()->Height;
	
	// (1)
	m_lpVBData[0].u = 0.0f;
	m_lpVBData[0].v = m_scale9Grid.m_top / height * v;
	m_lpVBData[1].u = 0.0f;
	m_lpVBData[1].v = 0.0f;
	m_lpVBData[2].u = m_scale9Grid.m_left / width * u;
	m_lpVBData[2].v = m_lpVBData[0].v;
	m_lpVBData[3].u = m_lpVBData[2].u;
	m_lpVBData[3].v = m_lpVBData[1].v;

	// (2)
	m_lpVBData[4].u = m_lpVBData[2].u;
	m_lpVBData[4].v = m_lpVBData[2].v;
	m_lpVBData[5].u = m_lpVBData[3].u;
	m_lpVBData[5].v = m_lpVBData[3].v;
	m_lpVBData[6].u = (width - m_scale9Grid.m_right) / width * u;
	m_lpVBData[6].v = m_lpVBData[4].v;
	m_lpVBData[7].u = m_lpVBData[6].u;
	m_lpVBData[7].v = m_lpVBData[5].v;

	// (3)
	m_lpVBData[8].u = m_lpVBData[6].u;
	m_lpVBData[8].v = m_lpVBData[6].v;
	m_lpVBData[9].u = m_lpVBData[7].u;
	m_lpVBData[9].v = m_lpVBData[7].v;
	m_lpVBData[10].u = u;
	m_lpVBData[10].v = m_lpVBData[6].v;
	m_lpVBData[11].u = m_lpVBData[10].u;
	m_lpVBData[11].v = m_lpVBData[9].v;

	//// (4)
	m_lpVBData[12].u = m_lpVBData[0].u;
	m_lpVBData[12].v = (height - m_scale9Grid.m_bottom) / height * v;
	m_lpVBData[13].u = m_lpVBData[0].u;
	m_lpVBData[13].v = m_lpVBData[0].v;
	m_lpVBData[14].u = m_lpVBData[2].u;
	m_lpVBData[14].v = m_lpVBData[12].v;
	m_lpVBData[15].u = m_lpVBData[2].u;
	m_lpVBData[15].v = m_lpVBData[2].v;

	// (5)
	m_lpVBData[16].u = m_lpVBData[14].u;
	m_lpVBData[16].v = m_lpVBData[14].v;
	m_lpVBData[17].u = m_lpVBData[15].u;
	m_lpVBData[17].v = m_lpVBData[15].v;
	m_lpVBData[18].u = m_lpVBData[6].u;
	m_lpVBData[18].v = m_lpVBData[14].v;
	m_lpVBData[19].u = m_lpVBData[6].u;
	m_lpVBData[19].v = m_lpVBData[6].v;
	
	// (6)
	m_lpVBData[20].u = m_lpVBData[18].u;
	m_lpVBData[20].v = m_lpVBData[18].v;
	m_lpVBData[21].u = m_lpVBData[19].u;
	m_lpVBData[21].v = m_lpVBData[19].v;
	m_lpVBData[22].u = m_lpVBData[10].u;
	m_lpVBData[22].v = m_lpVBData[18].v;
	m_lpVBData[23].u = m_lpVBData[10].u;
	m_lpVBData[23].v = m_lpVBData[10].v;

	// (7)
	m_lpVBData[24].u = m_lpVBData[12].u;
	m_lpVBData[24].v = v;
	m_lpVBData[25].u = m_lpVBData[12].u;
	m_lpVBData[25].v = m_lpVBData[12].v;
	m_lpVBData[26].u = m_lpVBData[14].u;
	m_lpVBData[26].v = m_lpVBData[24].v;
	m_lpVBData[27].u = m_lpVBData[14].u;
	m_lpVBData[27].v = m_lpVBData[14].v;

	// (8)
	m_lpVBData[28].u = m_lpVBData[26].u;
	m_lpVBData[28].v = m_lpVBData[26].v;
	m_lpVBData[29].u = m_lpVBData[27].u;
	m_lpVBData[29].v = m_lpVBData[27].v;
	m_lpVBData[30].u = m_lpVBData[18].u;
	m_lpVBData[30].v = m_lpVBData[28].v;
	m_lpVBData[31].u = m_lpVBData[18].u;
	m_lpVBData[31].v = m_lpVBData[18].v;

	// (9)
	m_lpVBData[32].u = m_lpVBData[30].u;
	m_lpVBData[32].v = m_lpVBData[30].v;
	m_lpVBData[33].u = m_lpVBData[20].u;
	m_lpVBData[33].v = m_lpVBData[20].v;
	m_lpVBData[34].u = m_lpVBData[22].u;
	m_lpVBData[34].v = m_lpVBData[32].v;
	m_lpVBData[35].u = m_lpVBData[22].u;
	m_lpVBData[35].v = m_lpVBData[22].v;

	// update xy
	static JGEVector2D point;

	// (1)
	point.m_x = -m_refX;
	point.m_y = -m_refY + m_lpVBData[0].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[0].x = point.m_x;
	m_lpVBData[0].y = point.m_y;

	point.m_x = -m_refX;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[1].x = point.m_x;
	m_lpVBData[1].y = point.m_y;

	point.m_x = -m_refX + m_lpVBData[2].u / u * width;
	point.m_y = -m_refY + m_lpVBData[2].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[2].x = point.m_x;
	m_lpVBData[2].y = point.m_y;

	point.m_x = -m_refX + m_lpVBData[3].u / u * width;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[3].x = point.m_x;
	m_lpVBData[3].y = point.m_y;

	// (2)
	m_lpVBData[4].x = m_lpVBData[2].x;
	m_lpVBData[4].y = m_lpVBData[2].y;

	m_lpVBData[5].x = m_lpVBData[3].x;
	m_lpVBData[5].y = m_lpVBData[3].y;

	point.m_x = -m_refX + m_lpVBData[6].u / u * width;
	point.m_y = -m_refY + m_lpVBData[6].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[6].x = point.m_x;
	m_lpVBData[6].y = point.m_y;

	point.m_x = -m_refX + m_lpVBData[7].u / u * width;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[7].x = point.m_x;
	m_lpVBData[7].y = point.m_y;

	// (3)
	m_lpVBData[8].x = m_lpVBData[6].x;
	m_lpVBData[8].y = m_lpVBData[6].y;

	m_lpVBData[9].x = m_lpVBData[7].x;
	m_lpVBData[9].y = m_lpVBData[7].y;

	point.m_x = -m_refX + m_lpVBData[10].u / u * width;
	point.m_y = -m_refY + m_lpVBData[10].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[10].x = point.m_x;
	m_lpVBData[10].y = point.m_y;

	point.m_x = -m_refX + m_lpVBData[11].u / u * width;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[11].x = point.m_x;
	m_lpVBData[11].y = point.m_y;
	
	// (4)
	point.m_x = -m_refX;
	point.m_y = -m_refY + m_lpVBData[12].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[12].x = point.m_x;
	m_lpVBData[12].y = point.m_y;

	m_lpVBData[13].x = m_lpVBData[0].x;
	m_lpVBData[13].y = m_lpVBData[0].y;

	point.m_x = -m_refX + m_lpVBData[14].u / u * width;
	point.m_y = -m_refY + m_lpVBData[14].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[14].x = point.m_x;
	m_lpVBData[14].y = point.m_y;

	m_lpVBData[15].x = m_lpVBData[2].x;
	m_lpVBData[15].y = m_lpVBData[2].y;

	// (5)
	m_lpVBData[16].x = m_lpVBData[14].x;
	m_lpVBData[16].y = m_lpVBData[14].y;

	m_lpVBData[17].x = m_lpVBData[15].x;
	m_lpVBData[17].y = m_lpVBData[15].y;

	point.m_x = -m_refX + m_lpVBData[18].u / u * width;
	point.m_y = -m_refY + m_lpVBData[18].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[18].x = point.m_x;
	m_lpVBData[18].y = point.m_y;

	m_lpVBData[19].x = m_lpVBData[6].x;
	m_lpVBData[19].y = m_lpVBData[6].y;

	// (6)
	m_lpVBData[20].x = m_lpVBData[18].x;
	m_lpVBData[20].y = m_lpVBData[18].y;

	m_lpVBData[21].x = m_lpVBData[19].x;
	m_lpVBData[21].y = m_lpVBData[19].y;

	point.m_x = -m_refX + m_lpVBData[22].u / u * width;
	point.m_y = -m_refY + m_lpVBData[22].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[22].x = point.m_x;
	m_lpVBData[22].y = point.m_y;

	m_lpVBData[23].x = m_lpVBData[10].x;
	m_lpVBData[23].y = m_lpVBData[10].y;

	// (7)
	point.m_x = -m_refX;
	point.m_y = -m_refY + m_lpVBData[24].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[24].x = point.m_x;
	m_lpVBData[24].y = point.m_y;

	m_lpVBData[25].x = m_lpVBData[12].x;
	m_lpVBData[25].y = m_lpVBData[12].y;

	point.m_x = -m_refX + m_lpVBData[26].u / u * width;
	point.m_y = -m_refY + m_lpVBData[26].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[26].x = point.m_x;
	m_lpVBData[26].y = point.m_y;

	m_lpVBData[27].x = m_lpVBData[14].x;
	m_lpVBData[27].y = m_lpVBData[14].y;

	// (8)
	m_lpVBData[28].x = m_lpVBData[26].x;
	m_lpVBData[28].y = m_lpVBData[26].y;

	m_lpVBData[29].x = m_lpVBData[27].x;
	m_lpVBData[29].y = m_lpVBData[27].y;

	point.m_x = -m_refX + m_lpVBData[30].u / u * width;
	point.m_y = -m_refY + m_lpVBData[30].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[30].x = point.m_x;
	m_lpVBData[30].y = point.m_y;

	m_lpVBData[31].x = m_lpVBData[18].x;
	m_lpVBData[31].y = m_lpVBData[18].y;

	// (9)
	m_lpVBData[32].x = m_lpVBData[30].x;
	m_lpVBData[32].y = m_lpVBData[30].y;

	m_lpVBData[33].x = m_lpVBData[31].x;
	m_lpVBData[33].y = m_lpVBData[31].y;

	point.m_x = -m_refX + m_lpVBData[34].u / u * width;
	point.m_y = -m_refY + m_lpVBData[34].v / v * height;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[34].x = point.m_x;
	m_lpVBData[34].y = point.m_y;

	m_lpVBData[35].x = m_lpVBData[22].x;
	m_lpVBData[35].y = m_lpVBData[22].y;

	// update color & alpha
	m_lpVBData[0].diffuse = (((int)(m_matrixGlobal.m_13 * 255.0f) & 0xFF) << 24) + (m_lpVBData[0].diffuse & 0xFFFFFF);
	for(UINT i = 1; i < 4 * m_numRenderRect; ++i)
	{
		m_lpVBData[i].diffuse = m_lpVBData[0].diffuse;
	}

	// render
	JGERender::getInstance()->renderDisplayObject(this);
}

JGEPoint* JGEScale9GridDisplayObject::getBoundsVectorGlobal(JGEPoint* lpBoundsResult)
{
	if(lpBoundsResult == null || m_lpVBData == null || m_lpTexture == null)
	{
		return null;
	}
	else
	{
		lpBoundsResult[0].m_x = m_lpVBData[24].x;
		lpBoundsResult[0].m_y = m_lpVBData[24].y;

		lpBoundsResult[1].m_x = m_lpVBData[1].x;
		lpBoundsResult[1].m_y = m_lpVBData[1].y;

		lpBoundsResult[2].m_x = m_lpVBData[11].x;
		lpBoundsResult[2].m_y = m_lpVBData[11].y;

		lpBoundsResult[3].m_x = m_lpVBData[34].x;
		lpBoundsResult[3].m_y = m_lpVBData[34].y;

		return lpBoundsResult;
	}
}