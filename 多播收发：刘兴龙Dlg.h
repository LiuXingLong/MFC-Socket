
// 多播收发：刘兴龙Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// C多播收发：刘兴龙Dlg 对话框
class C多播收发：刘兴龙Dlg : public CDialogEx
{
// 构造
public:
	C多播收发：刘兴龙Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
	CString str;
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEdit4();
	afx_msg void OnEnSetfocusEdit5();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
