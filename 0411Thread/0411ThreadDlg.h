
// 0411ThreadDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <windows.h>
#include <process.h>
// CMy0411ThreadDlg �Ի���
class CMy0411ThreadDlg : public CDialogEx
{
// ����
public:
	CMy0411ThreadDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY0411THREAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	HANDLE m_Event;//�¼�
	HANDLE m_hsemphore;//�ź���
};
