
// 0411ThreadDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <windows.h>
#include <process.h>
// CMy0411ThreadDlg 对话框
class CMy0411ThreadDlg : public CDialogEx
{
// 构造
public:
	CMy0411ThreadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY0411THREAD_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CButton m_threct;
	CListBox m_listbox;
	 long  m_shuliang;
	 afx_msg void OnClose();
	 CRITICAL_SECTION m_cs;
	HANDLE m_Mutex;
	HANDLE m_Event;//事件
	HANDLE m_hsemphore;//信号量
};
