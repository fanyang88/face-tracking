#pragma once
#include "Dib.h"
#include "LikelyHood.h"
#include <afxtempl.h>
#include <math.h>


class Process
{
public:
	 Process();
	~Process();

public:
    void MyDraw();
	void LoadOriPixel(CDib * pDib);
	void MakeBitMap();
	bool CopyBitMap(RGBQUAD ** dest,RGBQUAD ** source);
	void DrawCross(CDC *pDC, CPoint point, COLORREF crColor);
    bool intialize(CString strFile);
	void CallLikelyhood();
	void doBinary();
	void Verticalhistogram();
	void Horizonalhistogram();	
	CRect markface();

    CDC *pDCShow;//用来画图的区域
	CDib *m_pMainDib;//位图
	RGBQUAD** m_tOriPixelArray;//原始位图数据
	RGBQUAD** m_tResPixelArray;//参考位图数据

	CBitmap* m_pResMap;
	int m_nWndWidth;//图像宽度
	int m_nWndHeight;//图像高度
	CString m_sFileName;//位图名称

	bool m_bFaceOK;//脸部已经确定标志位
	bool m_bShowFace;
	CRect m_rFaceRegion;//脸部区域

	bool m_bSelectByMan;
	bool m_bLBottonDown;
	int m_ManLeft;
	int m_ManRight;
	int m_ManTop;
	int m_ManBottom;
    CString strPicPath;
	CBitmap m_bitmap;   //创建位图对象
	
	CLikelyHood	*method1;
};
