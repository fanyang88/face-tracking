// detectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "detection.h"
#include "detectionDlg.h"
#include "Process.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CdetectionDlg dialog
CdetectionDlg::CdetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdetectionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW, m_view);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_CLOSE, m_close);

	DDX_Control(pDX, IDC_VIDEO, m_picture);
	DDX_Control(pDX, IDC_PIC, m_show);
}

BEGIN_MESSAGE_MAP(CdetectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VIEW, &CdetectionDlg::OnBnClickedView)
	ON_BN_CLICKED(IDC_SAVE, &CdetectionDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CLOSE, &CdetectionDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_StartTrack, &CdetectionDlg::OnBnClickedStarttrack)
	ON_BN_CLICKED(IDC_StopTrack, &CdetectionDlg::OnBnClickedStoptrack)
	
END_MESSAGE_MAP()


// CdetectionDlg message handlers

BOOL CdetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

         if(! camera.OpenCamera(0, false, 640,480)) 
         return false;
         this->isView=false;
         this->isBreak = false;
         this->isSave = false;
		 pro= new Process();

////////////////////////////////////////////
	 
	b_flagTracking=false;
    imageTrack = NULL;
    hsvTrack = NULL;
    hueTrack = NULL;
    maskTrack = NULL;
    backprojectTrack = NULL;
    histimgTrack = NULL;
    histTrack = 0;  
    backproject_mode = 0;
    select_object = 0;
    track_object = 0;
    show_hist = 1;
    hdims = 100;
    vmin = 10;
    vmax = 256;
    smin = 30;
	b_flagProcess=false;
	Invalidate();
 
	     return TRUE;  // return TRUE  unless you set the focus to a control
}

void CdetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CdetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CdetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdetectionDlg::OnBnClickedView()
{   
	 this->isView=true;
	 SetTimer(1,10,NULL);
}


void CdetectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	int i, bin_w, c;
	this->m_picture.GetClientRect(&rect);  
         CDC* dc = this->m_picture.GetDC();
         HDC hDC=  dc->GetSafeHdc();
         int rw = rect.left;
          int rh = rect.top;
		  int iw =0, ih =0;

	switch (nIDEvent)
	{
	   case 1:
		if(this->isView)
	{		
		 this->m_pImage =camera.QueryFrame();
		 
		 if(m_pImage==NULL)
              return;
         this->m_picture.GetClientRect(&rect);  
         CDC* dc = this->m_picture.GetDC();
         HDC hDC=  dc->GetSafeHdc();
         int rw = rect.left;
          int rh = rect.top;
		 
		   iw = m_pImage->width;
           ih = m_pImage->height;
          SetRect(rect,rw,rh,rw+iw,rh+ih);
		
	     if (b_flagTracking==false)
	    {
		    cimg.CopyOf(m_pImage);
		    cimg.DrawToHDC(hDC,&rect);
		    ReleaseDC(dc);
	    }
	    b_flagProcess=true;

	if (b_flagTracking==false)
		return;

	cvCopy( m_pImage, imageTrack, 0 );
    cvCvtColor( imageTrack, hsvTrack, CV_BGR2HSV ); //change image from HSV to RGB	

	if( track_object)// if there is object needs to be tracked   		
	{
		int _vmin = vmin, _vmax = vmax;
		
		cvInRangeS( hsvTrack, cvScalar(0,smin,MIN(_vmin,_vmax),0),cvScalar(180,256,MAX(_vmin,_vmax),0), maskTrack ); 
		//make mask，only handle H：0~180，S：smin~256，V：vmin~vmax
		cvSplit( hsvTrack, hueTrack, 0, 0, 0 ); // get H
		
		if( track_object<0 )//if the feature hasn't been selected，do selection
		{
			float max_val = 0.f;
			cvSetImageROI( hueTrack, selection ); // set selection frame
			cvSetImageROI( maskTrack, selection ); // set mask selection frame
			
			cvCalcHist( &hueTrack, histTrack, 0, maskTrack ); // get histogram in the frame and satisfy mask
			
			cvGetMinMaxHistValue( histTrack, 0, &max_val, 0, 0 ); 
			cvConvertScale( histTrack->bins, histTrack->bins, max_val? 255. / max_val:0., 0 ); // histogram change to 0~255
			cvResetImageROI( hueTrack ); // remove ROI
			cvResetImageROI( maskTrack );
			track_window = selection;
			track_object = 1;
			
			cvZero( histimgTrack );
			bin_w = histimgTrack->width / hdims;
			
			for( i = 0; i < hdims; i++ )
			{
				int val = cvRound(cvGetReal1D(histTrack->bins,i)*histimgTrack->height/255 );
				CvScalar color = hsv2rgb(i*180.f/hdims);
				cvRectangle( histimgTrack, cvPoint(i*bin_w,histimgTrack->height),
				cvPoint((i+1)*bin_w,histimgTrack->height - val),color, -1, 8, 0 );//draw histogram
			}
        }
		
		cvCalcBackProject( &hueTrack, backprojectTrack, histTrack ); // get reflection image of hue
		
		cvAnd( backprojectTrack, maskTrack, backprojectTrack, 0 );//
		cvCamShift( backprojectTrack, track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),&track_comp, &track_box );//使用MeanShift算法对backproject中的内容进行搜索，返回跟踪结果
		track_window = track_comp.rect;//get the rectangle tracking frame
		
		if( backproject_mode )
			cvCvtColor( backprojectTrack, imageTrack, CV_GRAY2BGR ); // 
		if( imageTrack->origin )
			track_box.angle = -track_box.angle;
        cvEllipseBox( imageTrack, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );//draw tracking position
   }

	if( select_object && selection.width > 0 && selection.height > 0 )//during object selection phrase, draw frame
	{
		cvSetImageROI( imageTrack, selection );
		cvXorS( imageTrack, cvScalarAll(255), imageTrack, 0 );
		cvResetImageROI( imageTrack );
     }
	cimg.CopyOf(imageTrack);
    cimg.DrawToHDC(hDC, &rect);
   }
   
    break;

	    case 2:
	        if(this->isSave)
		       {
                this->m_pImage =camera.QueryFrame();
		         strFileName="data/2.bmp"; 		  
			      cimg.Save((LPTSTR)(LPCTSTR)strFileName);
				   this->m_show.GetClientRect(&rect);  
                   CDC* dc = this->m_show.GetDC();
                   pro->pDCShow=dc;
				   bool initial=pro->intialize(strFileName);
	               if(initial)
	                {
	                 pro->CallLikelyhood();
	                 pro->doBinary();
	                 pro->Verticalhistogram();
	                 pro->Horizonalhistogram();	
	                 face_region=pro->markface();
					 
                     selection.x=face_region.left;
	                 selection.y=face_region.top;
	                 selection.width=face_region.right-face_region.left;
	                 selection.height=face_region.bottom-face_region.top;

					////////////////////////////////////
   CPoint p1, p2,p3,p4;
	  p1.x=selection.x;
	  p1.y=selection.y;

	  p2.x=selection.x+selection.width;
	  p2.y=selection.y;

	  p3.x=selection.x;
	  p3.y=selection.y+selection.height;

	  p4.x=selection.x+selection.width;
	  p4.y=selection.y+selection.height;

	 
	  this->m_show.GetClientRect(&rect);  
         CDC* dc = this->m_show.GetDC();
         HDC hDC=  dc->GetSafeHdc();
      
		 CPen pen(PS_SOLID,3,RGB(0,255,255)); 
         CPen* pOldPen; 
         pOldPen=dc->SelectObject(&pen); 

           dc->MoveTo(p1); 
           dc->LineTo(p2);
           dc->MoveTo(p2); 
           dc->LineTo(p4);
		   dc->MoveTo(p1); 
           dc->LineTo(p3); 
           dc->MoveTo(p3); 
           dc->LineTo(p4);
           dc->SelectObject (&pOldPen);

					 //////////////////////////////////
	                // select_object = 0;
	                }			  
				 this->isSave=false;
		        }
			break;
			
		case 3:
                  if(this->isBreak)
		         {
				   delete pro;
	              KillTimer(ID_TIMER);  
		          this->isBreak = true;
                  this->OnClose();
		         }
	              break;
	}
	 __super::OnTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}


void CdetectionDlg::OnBnClickedSave()
{
	this->isSave=true;
	SetTimer(2,100,NULL);
}

void CdetectionDlg::OnBnClickedClose()
{
	this->isBreak=true;
	SetTimer(3,100,NULL);
}

void CdetectionDlg::OnBnClickedStarttrack()
{
	float hranges_arr[] = {0,180};//the range of pixel value
    float* hranges = hranges_arr; //use to initialize CvHistogram class
  
	if (!b_flagProcess)
	          return;

	b_flagTracking=true;
	imageTrack=0;
	track_object=-1;

		if (!imageTrack)
		{
			imageTrack = cvCreateImage( cvGetSize(this->m_pImage), 8, 3 );//
			imageTrack->origin = this->m_pImage->origin;
			hsvTrack = cvCreateImage( cvGetSize(this->m_pImage), 8, 3 );
			hueTrack = cvCreateImage( cvGetSize(this->m_pImage), 8, 1 );
			maskTrack = cvCreateImage( cvGetSize(this->m_pImage), 8, 1 );//locate mask image space
			backprojectTrack = cvCreateImage( cvGetSize(this->m_pImage), 8, 1 );//locate reflection image space, same size, single channel
			histTrack = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 ); //locate histogram space
			histimgTrack = cvCreateImage( cvSize(320,200), 8, 3 );//locate the space for drawing histogram
			cvZero( histimgTrack );//bg is black
		}
	origin = cvPoint(150,150);
    selection = cvRect(150,150,100,100);
}

void CdetectionDlg::OnBnClickedStoptrack()
{
	b_flagTracking=false;
	imageTrack=0;
	track_object=0;
	//kil timer 
	KillTimer(1);
	b_flagProcess=false;
}


CvScalar CdetectionDlg::hsv2rgb(float hue)
{
	int rgb[3], p, sector;
    static const int sector_data[][3]={{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;
	
    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;
	
    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}
