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

    CDC *pDCShow;//������ͼ������
	CDib *m_pMainDib;//λͼ
	RGBQUAD** m_tOriPixelArray;//ԭʼλͼ����
	RGBQUAD** m_tResPixelArray;//�ο�λͼ����

	CBitmap* m_pResMap;
	int m_nWndWidth;//ͼ����
	int m_nWndHeight;//ͼ��߶�
	CString m_sFileName;//λͼ����

	bool m_bFaceOK;//�����Ѿ�ȷ����־λ
	bool m_bShowFace;
	CRect m_rFaceRegion;//��������

	bool m_bSelectByMan;
	bool m_bLBottonDown;
	int m_ManLeft;
	int m_ManRight;
	int m_ManTop;
	int m_ManBottom;
    CString strPicPath;
	CBitmap m_bitmap;   //����λͼ����
	
	CLikelyHood	*method1;
};
