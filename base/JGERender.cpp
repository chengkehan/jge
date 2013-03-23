#include "JGERender.h"

JGE_SINGLETON_IMPLEMENTS(JGERender)

JGERender::JGERender()
{
	m_lpd3dd = null;
	m_lpVbBuffer = null;
	m_lpVbBufferData = null;
	m_lpIbBuffer = null;
	m_lpIbBufferData = null;
	m_lpTextureCurrent = null;
	m_bufferDisplayObjectAmount = 0;
	m_displayObjectCount = 0;
	m_alphaEnbaledCurrent = false;
	m_vbibLocked = false;
}

JGERender::~JGERender()
{
	jgeReleaseCom(m_lpVbBuffer);
	jgeReleaseCom(m_lpIbBuffer);
}

bool JGERender::init(IDirect3DDevice9* lpd3dd, uint bufferDisplayObjectAmount)
{
	if(lpd3dd != null && bufferDisplayObjectAmount > 0 && m_lpVbBuffer == null && m_lpIbBuffer == null)
	{
		m_lpd3dd = lpd3dd;
		m_bufferDisplayObjectAmount = bufferDisplayObjectAmount;
		if(FAILED(m_lpd3dd->CreateVertexBuffer(
			m_bufferDisplayObjectAmount * 4 * sizeof(JGEDisplayObject::Vertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, JGEDisplayObject::Vertex::FVF, D3DPOOL_DEFAULT, &m_lpVbBuffer, null
		)))
		{
			return false;
		}
		if(FAILED(m_lpd3dd->CreateIndexBuffer(
			m_bufferDisplayObjectAmount * 6 * sizeof(word), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_lpIbBuffer, null
		)))
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void JGERender::beginScene()
{
	unlockVbIb();
	lockVbIb();
	m_displayObjectCount = 0;
	m_lpTextureCurrent = null;
	m_alphaEnbaledCurrent = false;
}

bool JGERender::renderDisplayObject(JGEDisplayObject* lpDisplayObject)
{
	if(lpDisplayObject == null || lpDisplayObject->getTexture() == null || lpDisplayObject->getTexture()->getTexture() == null)
	{
		return false;
	}
	else
	{
		if(m_lpTextureCurrent != null && (lpDisplayObject->getTexture()->getTexture() != m_lpTextureCurrent || lpDisplayObject->getAlphaEnabled() != m_alphaEnbaledCurrent || m_displayObjectCount + lpDisplayObject->m_numRenderRect > m_bufferDisplayObjectAmount))
		{
			renderBuffer(true);
		}
		m_lpTextureCurrent = lpDisplayObject->getTexture()->getTexture();
		m_alphaEnbaledCurrent = lpDisplayObject->getAlphaEnabled();
		jgeMemCpy(lpDisplayObject->m_lpVBData, &m_lpVbBufferData[m_displayObjectCount * 4], sizeof(JGEDisplayObject::Vertex) * 4 * lpDisplayObject->m_numRenderRect);
		for (UINT i = 0; i < lpDisplayObject->m_numRenderRect; ++i)
		{
			m_lpIbBufferData[m_displayObjectCount * 6] = m_displayObjectCount * 4;
			m_lpIbBufferData[m_displayObjectCount * 6 + 1] = m_displayObjectCount * 4 + 1;
			m_lpIbBufferData[m_displayObjectCount * 6 + 2] = m_displayObjectCount * 4 + 2;
			m_lpIbBufferData[m_displayObjectCount * 6 + 3] = m_displayObjectCount * 4 + 3;
			m_lpIbBufferData[m_displayObjectCount * 6 + 4] = m_displayObjectCount * 4 + 2;
			m_lpIbBufferData[m_displayObjectCount * 6 + 5] = m_displayObjectCount * 4 + 1;
			++m_displayObjectCount;
		}
		return true;
	}
}

void JGERender::endScene()
{
	renderBuffer(false);	
}

void JGERender::renderBuffer(bool relock)
{
	unlockVbIb();
	if(m_displayObjectCount > 0)
	{
		m_lpd3dd->SetTexture(0, m_lpTextureCurrent);
		if(m_alphaEnbaledCurrent)
		{
			m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_lpd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		else
		{
			m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		}
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
		m_lpd3dd->SetStreamSource(0, m_lpVbBuffer, 0, sizeof(JGEDisplayObject::Vertex));
		m_lpd3dd->SetIndices(m_lpIbBuffer);
		m_lpd3dd->SetFVF(JGEDisplayObject::Vertex::FVF);
		m_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_displayObjectCount * 4, 0, m_displayObjectCount * 2);
	}
	if(relock)
	{
		lockVbIb();
	}
	m_displayObjectCount = 0;
	m_lpTextureCurrent = null;
	m_alphaEnbaledCurrent = false;
}

void JGERender::lockVbIb()
{
	if(!m_vbibLocked)
	{
		m_lpVbBuffer->Lock(0, m_bufferDisplayObjectAmount * 4 * sizeof(JGEDisplayObject::Vertex), (void**)&m_lpVbBufferData, D3DLOCK_DISCARD);
		m_lpIbBuffer->Lock(0, m_bufferDisplayObjectAmount * 6 * sizeof(word), (void**)&m_lpIbBufferData, D3DLOCK_DISCARD);
		m_vbibLocked = true;
	}
}

void JGERender::unlockVbIb()
{
	if(m_vbibLocked)
	{
		m_lpVbBuffer->Unlock();
		m_lpIbBuffer->Unlock();
		m_vbibLocked = false;
	}
}