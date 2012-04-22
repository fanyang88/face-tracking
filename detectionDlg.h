// detectionDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "CameraDS.h"
#include <vector>
#include "Process.h"

// CdetectionDlg dialog
class CdetectionDlg : public CDialog
{
// Construction
public:
	CdetectionDlg(CWnd* pParent = NULL);	// standard constructor
    
         CCameraDS  camera;
         IplImage* m_pImage;  
         bool isBreak;
		 bool isSave;
		 bool isView;
         CString strFileName;
		 CvvImage cimg;
         CRect rect;
         Process* pro;

	bool start;
     CRect face_region;
    bool b_flagProcess;  
    bool b_flagTracking;
  IplImage *imageTrack;
  IplImage *hsvTrack;
  IplImage *hueTrack;
  IplImage *maskTrack;
  IplImage *backprojectTrack;
  IplImage *histimgTrack;//use the Hue vector in HSV to track
  CvHistogram *histTrack;//histogram
  int backproject_mode;
  int select_object;
  int track_object;
  int show_hist;
  CvPoint origin;
  CvRect selection;
  CvRect track_window;
  CvBox2D track_box; // the box class return by Meanshift tracking AL
  CvConnectedComp track_comp;
  int hdims; // the number of bins in histogram, the more the accurate
  int vmin, vmax, smin;

// Dialog Data
	enum { IDD = IDD_DETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CButton m_camera;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CButton m_view;
	afx_msg void OnBnClickedView();
	CButton m_takePhoto;
	CButton m_save;
	CButton m_close;
	CStatic m_picture;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedClose();
	CStatic m_show;
	afx_msg void OnBnClickedStarttrack();
	afx_msg void OnBnClickedStoptrack();	
	CvScalar hsv2rgb(float hue);

};
