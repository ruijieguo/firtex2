// TestAppDlg.h : header file
//

#if !defined(AFX_CPPUNITTESTAPPDLG_H__25E1CF20_72A4_4E25_B930_626DF60AD4C7__INCLUDED_)
#define AFX_CPPUNITTESTAPPDLG_H__25E1CF20_72A4_4E25_B930_626DF60AD4C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TestAppDlg dialog

class TestAppDlg : public CDialog
{
// Construction
public:
	TestAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(TestAppDlg)
	enum { IDD = IDD_CPPUNITTESTAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TestAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(TestAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPPUNITTESTAPPDLG_H__25E1CF20_72A4_4E25_B930_626DF60AD4C7__INCLUDED_)
