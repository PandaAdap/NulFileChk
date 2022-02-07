
// NulFileChkDlg.h: 头文件
//

#pragma once
#include <vector>

// CNulFileChkDlg 对话框
class CNulFileChkDlg : public CDialogEx
{
// 构造
public:
	CNulFileChkDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NULFILECHK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnJaropen();
	CListCtrl m_filereport;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnNMRClickFilereport(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRcmenuDelete();
	afx_msg void OnRcmenuCheck();
	afx_msg void OnBnClickedBtnClean();
};
