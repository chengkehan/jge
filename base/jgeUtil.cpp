#include "jgeUtil.h"

char __jge_trace_buffer[1024];
DWORD __jge_timePoint;

char* jgecsclone(const char* lpSrc, char* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgecslen(lpSrc) + 1/*'\0'*/;
	char* lpStrClone = null;
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen, char*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen);
	return lpStrClone;
}

wchar_t* jgewcsclone(const wchar_t* lpSrc, wchar_t* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgewcslen(lpSrc) + 1/*'\0'*/;
	wchar_t* lpStrClone = null;
	// strLen * 2 是因为wchar占用的空间是char的两倍
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen * 2, wchar_t*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen * 2);
	return lpStrClone;
}

void jgeWin32GetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult, int* lpYResult, uint* lpWidthResult, uint* lpHeightResult)
{
	uint widthResult = widthSrc + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	uint heightResult = heightSrc + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	if(lpWidthResult != null)
	{
		*lpWidthResult = widthResult;
	}
	if(lpHeightResult != null)
	{
		*lpHeightResult = heightResult;
	}
	if(lpXResult != null)
	{
		*lpXResult = (int)((GetSystemMetrics(SM_CXSCREEN) - widthResult) * 0.5f);
	}
	if(lpYResult != null)
	{
		*lpYResult = (int)((GetSystemMetrics(SM_CYSCREEN) - heightResult) * 0.5f);
	}
}

//设有向线段AB，两端点A（xa,ya）,B(xb,yb)
//另一点C(xc,yc)
//float f = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);
//if(f > 0)
//	点C位于有向线段AB的左侧
//else if(f == 0)
//点C位于有向线段AB上（也有可能在延长线上）
//else
//点C位于有向线段AB的右侧
int jgeVectorABPointSide(JGEPoint* lpPointA, JGEPoint* lpPointB, JGEPoint* lpPoint)
{
	if(lpPointA == null || lpPointB == null || lpPoint == null)
	{
		return -2;
	}
	
	float f = (lpPointB->m_x - lpPointA->m_x) * (lpPoint->m_y - lpPointA->m_y) - (lpPoint->m_x - lpPointA->m_x) * (lpPointB->m_y - lpPointA->m_y);
	if(f > 0.0f)
	{
		return 1;
	}
	else if(jgeFloatEqual(f, 0.0f))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

JGEVector2D* jgeVector2DTransform(JGEVector2D* lpVector, const JGEMatrix2D* lpMatrix)
{
	if(lpVector == null || lpMatrix == null)
	{
		return null;
	}

	static JGEVector2D vector;
	vector.m_x = lpVector->m_x;
	vector.m_y = lpVector->m_y;
	vector.m_w = lpVector->m_w;
	lpVector->m_x = vector.m_x * lpMatrix->m_11 + vector.m_y * lpMatrix->m_21 + vector.m_w * lpMatrix->m_31;
	lpVector->m_y = vector.m_x * lpMatrix->m_12 + vector.m_y * lpMatrix->m_22 + vector.m_w * lpMatrix->m_32;

	return lpVector;
}

JGEMatrix2D* jgeMatrix2DDotProduct(const JGEMatrix2D* lpMat1, const JGEMatrix2D* lpMat2, JGEMatrix2D* lpMatResult)
{
	if(lpMat1 == null || lpMat2 == null || lpMatResult == null)
	{
		return null;
	}

	static JGEMatrix2D mat1;
	mat1.m_11 = lpMat1->m_11; mat1.m_12 = lpMat1->m_12; mat1.m_13 = lpMat1->m_13;
	mat1.m_21 = lpMat1->m_21; mat1.m_22 = lpMat1->m_22; mat1.m_23 = lpMat1->m_23;
	mat1.m_31 = lpMat1->m_31; mat1.m_32 = lpMat1->m_32; mat1.m_33 = lpMat1->m_33;
	static JGEMatrix2D mat2;
	mat2.m_11 = lpMat2->m_11; mat2.m_12 = lpMat2->m_12; mat2.m_13 = lpMat2->m_13;
	mat2.m_21 = lpMat2->m_21; mat2.m_22 = lpMat2->m_22; mat2.m_23 = lpMat2->m_23;
	mat2.m_31 = lpMat2->m_31; mat2.m_32 = lpMat2->m_32; mat2.m_33 = lpMat2->m_33;

	lpMatResult->m_11 = mat1.m_11 * mat2.m_11 + mat1.m_12 * mat2.m_21 + mat1.m_13 * mat2.m_31;
	lpMatResult->m_12 = mat1.m_11 * mat2.m_12 + mat1.m_12 * mat2.m_22 + mat1.m_13 * mat2.m_32;
	lpMatResult->m_13 = 0.0f;

	lpMatResult->m_21 = mat1.m_21 * mat2.m_11 + mat1.m_22 * mat2.m_21 + mat1.m_23 * mat2.m_31;
	lpMatResult->m_22 = mat1.m_21 * mat2.m_12 + mat1.m_22 * mat2.m_22 + mat1.m_23 * mat2.m_32;
	lpMatResult->m_23 = 0.0f;

	lpMatResult->m_31 = mat1.m_31 * mat2.m_11 + mat1.m_32 * mat2.m_21 + mat1.m_33 * mat2.m_31;
	lpMatResult->m_32 = mat1.m_31 * mat2.m_12 + mat1.m_32 * mat2.m_22 + mat1.m_33 * mat2.m_32;
	lpMatResult->m_33 = 1.0f;

	return lpMatResult;
}

JGEMatrix2D* jgeMatrix2DIdentity(JGEMatrix2D* lpMatrix)
{
	if(lpMatrix == null)
	{
		return null;
	}

	jgeZeroMem(lpMatrix->m_data, sizeof(float) * 9);
	lpMatrix->m_11 = 1.0f; lpMatrix->m_22 = 1.0f; lpMatrix->m_33 = 1.0f;

	return lpMatrix;
}

JGEMatrix2D* jgeMatrix2DRotation(JGEMatrix2D* lpMatrix, float radian)
{
	if(lpMatrix == null)
	{
		return null;
	}

	float cosValue = cosf(radian);
	float sinValue = sinf(radian);
	lpMatrix->m_11 = cosValue; lpMatrix->m_12 = sinValue;
	lpMatrix->m_21 = -sinValue; lpMatrix->m_22 = cosValue;
	lpMatrix->m_31 = 0.0f; lpMatrix->m_32 = 0.0f;

	return lpMatrix;
}

JGEMatrix2D* jgeMatrix2DTranslation(JGEMatrix2D* lpMatrix, float x, float y)
{
	if(lpMatrix == null)
	{
		return null;
	}

	jgeZeroMem(lpMatrix->m_data, sizeof(float) * 9);
	lpMatrix->m_11 = 1.0f; lpMatrix->m_22 = 1.0f;
	lpMatrix->m_31 = x;
	lpMatrix->m_32 = y;

	return lpMatrix;
}

JGEMatrix2D* jgeMatrix2DScaling(JGEMatrix2D* lpMatrix, float sx, float sy)
{
	if(lpMatrix == null)
	{
		return null;
	}

	jgeZeroMem(lpMatrix->m_data, sizeof(float) * 9);
	lpMatrix->m_11 = sx;
	lpMatrix->m_22 = sy;

	return lpMatrix;
}

JGEMatrix2D* jgeMatrix2DRotationScalingTranslationDotProductAlpha(float radian, float sx, float sy, float x, float y, const JGEMatrix2D* lpMatrix, float alpha, JGEMatrix2D* lpMatrixResult)
{
	if(lpMatrixResult == null)
	{
		return null;
	}

	static JGEMatrix2D matrixRotation;
	jgeMatrix2DRotation(&matrixRotation, radian);
	static JGEMatrix2D matrixScaling;
	jgeMatrix2DScaling(&matrixScaling, sx, sy);
	static JGEMatrix2D matrixTranslation;
	jgeMatrix2DTranslation(&matrixTranslation, x, y);

	jgeMatrix2DDotProduct(&matrixRotation, &matrixScaling, lpMatrixResult);
	jgeMatrix2DDotProduct(lpMatrixResult, &matrixTranslation, lpMatrixResult);

	if(lpMatrix == null)
	{
		lpMatrixResult->m_13 = alpha;
	}
	else
	{
		jgeMatrix2DDotProduct(lpMatrixResult, lpMatrix, lpMatrixResult);
		lpMatrixResult->m_13 = lpMatrix->m_13 * alpha;
	}

	return lpMatrixResult;
}

D3DXMATRIX* jgeMatrix2DToD3DXMatrix(const JGEMatrix2D* lpMatrix, D3DXMATRIX* lpD3DMatrix)
{
	if(lpMatrix == null || lpD3DMatrix == null)
	{
		return null;
	}

	D3DXMatrixIdentity(lpD3DMatrix);
	lpD3DMatrix->_11 = lpMatrix->m_11; lpD3DMatrix->_12 = lpMatrix->m_12; lpD3DMatrix->_14 = lpMatrix->m_13;
	lpD3DMatrix->_21 = lpMatrix->m_21; lpD3DMatrix->_22 = lpMatrix->m_22; lpD3DMatrix->_24 = lpMatrix->m_23;
	lpD3DMatrix->_41 = lpMatrix->m_31; lpD3DMatrix->_42 = lpMatrix->m_32; lpD3DMatrix->_44 = lpMatrix->m_33;

	return lpD3DMatrix;
}

bool jgeReadFile(const char* lpFile, char* lpFileData, uint* lpFileDataBytes, bool isBinary)
{
	std::ifstream reader;
	reader.open(lpFile, std::ios_base::in | (isBinary ? std::ios_base::binary : 0));
	bool r = false;
	if(reader.good())
	{
		reader.seekg(0, std::ios_base::end);
		UINT bytes = (UINT)reader.tellg();
		reader.seekg(0, std::ios_base::beg);
		if(lpFileDataBytes != null)
		{
			*lpFileDataBytes = bytes;
		}
		if(lpFileData != null)
		{
			jgeZeroMem(lpFileData, bytes);
			reader.read(lpFileData, bytes);
		}
		r = true;
	}
	reader.close();

	return r;
}