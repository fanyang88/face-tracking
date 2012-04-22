#include "StdAfx.h"
#include "Process.h"

Process::Process()
{
	m_pMainDib = new CDib();
	m_tOriPixelArray = NULL;
	m_tResPixelArray = NULL;
	m_pResMap = NULL;
	m_nWndWidth = 0;
	m_nWndHeight= 0;
	m_sFileName = "";
	m_bSelectByMan = false;
	m_bLBottonDown = false;
	m_ManLeft = -1;
	m_ManRight = -1;
	m_ManTop = -1;
	m_ManBottom = -1;
	m_bFaceOK = false;
	m_bShowFace = false;
	m_rFaceRegion.left =0;
	m_rFaceRegion.right =0;
	m_rFaceRegion.top =0;
	m_rFaceRegion.bottom = 0;
}

Process::~Process(void)
{
	//if(!m_pMainDib)
	//	m_pMainDib->Close();
	m_tOriPixelArray = NULL;
	m_tResPixelArray = NULL;
	m_pResMap = NULL;
	m_nWndWidth = 0;
	m_nWndHeight= 0;
	m_sFileName = "";
	m_bSelectByMan = false;
	m_bLBottonDown = false;
	m_ManLeft = -1;
	m_ManRight = -1;
	m_ManTop = -1;
	m_ManBottom = -1;
	m_bFaceOK = false;
	m_bShowFace = false;
	m_rFaceRegion.left =0;
	m_rFaceRegion.right =0;
	m_rFaceRegion.top =0;
	m_rFaceRegion.bottom = 0;
}


void Process::LoadOriPixel(CDib *pDib)
{
	BYTE	*colorTable;
	colorTable = (BYTE *)pDib->m_pDibBits;
	int byteBitCount  = pDib->GetBiBitCount()/8;

	m_tOriPixelArray  = new RGBQUAD*[m_nWndHeight];
	m_tResPixelArray  = new RGBQUAD*[m_nWndHeight];
	for(int l=0 ; l<m_nWndHeight; l++)
	{
		m_tOriPixelArray[l] = new RGBQUAD[m_nWndWidth];
		m_tResPixelArray[l] = new RGBQUAD[m_nWndWidth];
	}

	int count = 0;
	for(int i=m_nWndHeight-1; i>=0; i--)
	{
		for(int j=0; j<m_nWndWidth; j++)
		{
			m_tOriPixelArray[i][j].rgbBlue =colorTable[count++];
			m_tOriPixelArray[i][j].rgbGreen=colorTable[count++];
			m_tOriPixelArray[i][j].rgbRed  =colorTable[count++];
			m_tOriPixelArray[i][j].rgbReserved = 0;
			m_tResPixelArray[i][j]=m_tOriPixelArray[i][j];
			count += byteBitCount-3;
		}
		count += (4-(m_nWndWidth*byteBitCount)%4)%4;
	}
     method1 = new CLikelyHood(m_tOriPixelArray,m_nWndWidth,m_nWndHeight);
}

void Process::MyDraw()
{
	CRect rc;
	pDCShow->GetWindow()->GetClientRect(&rc);
	pDCShow->Rectangle(&rc);
	CDC dc;
	CBitmap *pOldBitmap;
	dc.CreateCompatibleDC(pDCShow);
	pOldBitmap=dc.SelectObject(m_pResMap);
	pDCShow->StretchBlt(0,0,m_nWndWidth,m_nWndHeight,&dc,0,0,m_nWndWidth,m_nWndHeight,SRCCOPY);
	dc.SelectObject(pOldBitmap);
	dc.DeleteDC(); 
}

void Process::MakeBitMap()
{
	CClientDC ClientDC(pDCShow->GetWindow());
	if(m_pResMap!=NULL) 
		delete m_pResMap;

	m_pResMap=new CBitmap();
	m_pResMap->CreateCompatibleBitmap(&ClientDC,m_nWndWidth,m_nWndHeight);

	CDC dc;
	dc.CreateCompatibleDC(&ClientDC);
	dc.SelectObject(m_pResMap);

	for(int i=0; i<m_nWndHeight; i++)
	for(int j=0; j<m_nWndWidth; j++)
	dc.SetPixelV(j,i,RGB(m_tResPixelArray[i][j].rgbRed,m_tResPixelArray[i][j].rgbGreen,m_tResPixelArray[i][j].rgbBlue));

	if(m_bFaceOK && m_bShowFace)
	{
		CBrush Pen;
		Pen.CreateSolidBrush(RGB(255,0,0));
		dc.FrameRect(m_rFaceRegion,&Pen);
	 	Pen.DeleteObject();
	}

	dc.DeleteDC();
	MyDraw();
}


bool Process::CopyBitMap(RGBQUAD **dest, RGBQUAD **source)
{
	if(source==NULL || dest==NULL) 
		return false;
	for(int i=0; i<m_nWndHeight; i++)
		for(int j=0; j<m_nWndWidth; j++)
			dest[i][j]=source[i][j];
	
	return true;
}

void Process::DrawCross(CDC *pDC, CPoint point, COLORREF crColor)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,crColor);
	oldPen = (CPen*)pDC->SelectObject(&pen);
	pDC->MoveTo(point.x-7,point.y);
	pDC->LineTo(point.x+7,point.y);
	pDC->MoveTo(point.x,point.y-7);
	pDC->LineTo(point.x,point.y+7);
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
}

bool Process::intialize(CString strFile)
{
	this->m_pMainDib->Open(strFile);
	if(!m_pMainDib)
		return false;

	this->m_nWndWidth = this->m_pMainDib->GetWidth();
	this->m_nWndHeight= this->m_pMainDib->GetHeight();
	m_sFileName = strFile;

	m_rFaceRegion.left = 0;
	m_rFaceRegion.right =0;
	m_rFaceRegion.top = 0;
	m_rFaceRegion.bottom = 0;
	//get pixel values
	LoadOriPixel(this->m_pMainDib);
	return true;
}

void Process::CallLikelyhood()
{
    m_bShowFace = false;
	method1->CalLikeHood();
	for(int i=0; i<m_nWndHeight; i++)
	for(int j=0; j<m_nWndWidth;  j++)
	{
		m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
		m_tResPixelArray[i][j].rgbRed  = (int)(method1->m_pLikeliHoodArray[i][j]*255);
	}
}

void Process::doBinary()
{
if(!method1->CalBinary())
	{
		AfxMessageBox("calculate the binary£¡");	
		return;
	}
	
	m_bShowFace = false;
	for(int i=0; i<m_nWndHeight; i++)
	for(int j=0; j<m_nWndWidth;  j++)
	{
		m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
		m_tResPixelArray[i][j].rgbRed  = (int)(method1->m_pBinaryArray[i][j]*255);
	}	
}
	
void Process::Verticalhistogram()
{
    if(!method1->m_bBinaryReady)
	{
		AfxMessageBox("calculate the binary!");
		return;
	}
	m_bShowFace = false;
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	for(int i=0; i<m_nWndHeight; i++)		
	{
		int count = 0;
		for(int j=0; j<m_nWndWidth;  j++)
		{
			if(method1->m_pBinaryArray[i][j] == 1) count++;
			m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
			m_tResPixelArray[i][j].rgbRed  = 255;
		}
		for(int j=0; j<count;  j++)
		{
			m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
			m_tResPixelArray[i][j].rgbRed  = 0;
		}
	}	
}

void Process::Horizonalhistogram()
{
   if(!method1->m_bBinaryReady)
	{
		AfxMessageBox("calculate binary first");
		return;
	}
	m_bShowFace = false;
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	for(int j=0; j<m_nWndWidth;  j++)	
	{
		int count = 0;
		for(int i=0; i<m_nWndHeight; i++)
		{
			if(method1->m_pBinaryArray[i][j] == 1) count++;
			m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
			m_tResPixelArray[i][j].rgbRed  = 255;
		}
		for(int i=m_nWndHeight-1; i>=m_nWndHeight-count;i--)
		{
			m_tResPixelArray[i][j].rgbBlue = m_tResPixelArray[i][j].rgbGreen =	
			m_tResPixelArray[i][j].rgbRed  = 0;
		}
	}	
}

CRect Process::markface()
{
   if(!method1->m_bBinaryReady)
	{
		AfxMessageBox("calculate binary first£¡");
		return NULL;
	}
	m_bShowFace = true;
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	int *temp = new int[m_nWndWidth];
	int max = 0;
	int pos = -1;
	for(int j=0; j<m_nWndWidth;  j++)	
	{
		int count = 0;
		for(int i=0; i<m_nWndHeight; i++)
		{
			if(method1->m_pBinaryArray[i][j] == 1) count++;
		}
		temp[j] = count;
		if(count > max)
		{
			max = count;
			pos = j;
		}
	}
	int left=0,right=0,l,top=0,bottom=0;
	for(l=pos; l>=0; l--)
	{
		if(temp[l]<max*0.2||l==0)
		{
			left = l;
			break;
		}
	}
	for(l=pos; l<m_nWndWidth; l++)
	{
		if(temp[l]<max*0.3||l==m_nWndWidth-1)
		{
			right = l;
			break;
		}
	}
	for(int i=0; i<m_nWndHeight; i++)
	{
		int count = 0;
		for(l = left;l<=right;l++)
		{
			if(method1->m_pBinaryArray[i][l] == 1) count++;
		}
		if(count>=(right-left)*0.5)
		{
			top = i;
			break;
		}
	}
	bottom = (int)(top+(right-left)*1.5)>=m_nWndHeight? m_nWndHeight-1:(int)(top+(right-left)*1.5);

	CopyBitMap(m_tResPixelArray,m_tOriPixelArray);

	for(int i=top;i<=bottom;i++)
	{
		m_tResPixelArray[i][left].rgbBlue=255;
		m_tResPixelArray[i][left].rgbGreen = m_tResPixelArray[i][left].rgbRed = 0;
		m_tResPixelArray[i][right].rgbBlue=255;
		m_tResPixelArray[i][right].rgbGreen = m_tResPixelArray[i][right].rgbRed = 0;
	}
	for(int j=left;j<=right;j++)
	{
		m_tResPixelArray[top][j].rgbBlue=255;
		m_tResPixelArray[top][j].rgbGreen = m_tResPixelArray[top][j].rgbRed = 0;
		m_tResPixelArray[bottom][j].rgbBlue=255;
		m_tResPixelArray[bottom][j].rgbGreen = m_tResPixelArray[bottom][j].rgbRed = 0;
	}

	if(m_bFaceOK)
	{	
			CopyBitMap(m_tResPixelArray,m_tOriPixelArray);
			CRect rect(left,top,right,bottom);
			m_rFaceRegion = rect;
	}
	else
	{
		m_bFaceOK = true;
		CopyBitMap(m_tResPixelArray,m_tOriPixelArray);
		CRect rect(left,top,right,bottom);
		m_rFaceRegion = rect;	
	}
	MakeBitMap();

	return m_rFaceRegion;
}




