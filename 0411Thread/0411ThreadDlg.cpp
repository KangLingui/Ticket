
// 0411ThreadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "0411Thread.h"
#include "0411ThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy0411ThreadDlg �Ի���



CMy0411ThreadDlg::CMy0411ThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy0411ThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_shuliang = 100;
	InitializeCriticalSection(&m_cs);//�ؼ���
	m_Mutex = CreateMutex(0,0,0);//������
	m_Event = CreateEvent(0,//��ȫ����
		false,//�¼�false �����Զ��¼�  true  �˹��¼�
		true,//��λ���ź�
		NULL);
	 m_hsemphore = CreateSemaphore(0,
		 1,//��ʼ������
		 10,//�ź������ĸ���
		 0
		 
		 );//�ź���

}

void CMy0411ThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_threct);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
}

BEGIN_MESSAGE_MAP(CMy0411ThreadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMy0411ThreadDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMy0411ThreadDlg ��Ϣ�������

BOOL CMy0411ThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMy0411ThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy0411ThreadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy0411ThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//�߳�ͬ��  ----����̲߳���������
//1.ԭ�ӷ��� ---- ͬһʱ��ֻ����һ���̷߳�����Դ����������ʵ�ַ�ʽ��Inclock��
//volatile  ��ֹ�����Ż����������ַ���ȶ�����
//2.�ؼ��Σ��û�ģʽ�µ��߳�ͬ����-------����ʼ����ֹ  �м�Ĵ���ֻ����һ���߳���
//���У�����ȥ������-����-����ڿ��Խ�ȥ������-����2.1��ֱ��������
//��һ�ᣨ�ܽ�ȥ�ͽ�ȥ������ȥ������̬��2.2��ת��
//2.3�첽������ԶԸ���ڣ�(��ͼ��ȥ������ȥ�������������)
//3(�ں˶���) 
		//3.1������--ͬһʱ������һ���̷߳��ʴ����
		// ����1���������Կ���̣�2��ȫ��3Ч�ʣ��ؼ��θߣ��ں�ģʽ��Ҫ�л� 4�������������ó�ʼӵ��Ȩ
		//3.2  �¼����ں˶���--������--ͬһʱ������һ���̷߳��ʴ����
		//    �˹��¼����Զ��¼�
		//3.3�ź��� --------�и�������




//�̺߳���
DWORD WINAPI ThreadPro(LPVOID lppar)
{

//Ʊ������
	CMy0411ThreadDlg *pthis = (CMy0411ThreadDlg*)lppar;
	CString str;
	while (1)
	{
		if(pthis->m_shuliang==0)
		{
			str.Format(_T("%d������"),GetCurrentThreadId());
			pthis->m_listbox.AddString(str);
		break;
		}
			


		Sleep(10);
			WaitForSingleObject(pthis->m_hsemphore,INFINITE);//�ź���--���
					//WaitForSingleObject(pthis->m_Event,INFINITE);//�¼�--���
							//WaitForSingleObject(pthis->m_Mutex,INFINITE);//������--���
									//EnterCriticalSection(&pthis->m_cs);//�ؼ���--��ʼ
		
												if(pthis->m_shuliang==0)
												{
														str.Format(_T("%d����"),GetCurrentThreadId());
														pthis->m_listbox.AddString(str);
														//LeaveCriticalSection(&pthis->m_cs);//�ؼ���--����
														//ReleaseMutex(pthis->m_Mutex);//������--����
														//SetEvent(pthis->m_Event);//�¼�--��λ���ź�
														ReleaseSemaphore(pthis->m_hsemphore,1,0);//�ź���
														break;
												}
												str.Format(_T("%d�������%d�ų�Ʊ"),GetCurrentThreadId(),pthis->m_shuliang);
												//InterlockedDecrement(&pthis->m_shuliang);//ԭ�ӷ���
												pthis->m_shuliang--;
									//LeaveCriticalSection(&pthis->m_cs);//�ؼ���--����
							//ReleaseMutex(pthis->m_Mutex);//������--����
					//SetEvent(pthis->m_Event);//�¼�--��λ���ź�
			ReleaseSemaphore(pthis->m_hsemphore,1,0);//�ź���--����

		pthis->m_listbox.AddString(str);
	}
    return 0;
}




void CMy0411ThreadDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����10���߳�
	

	for(int i = 0;i<10 ;i++)
	{
	CreateThread(NULL,0,&ThreadPro,this,0,0);

	}
}


void CMy0411ThreadDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DeleteCriticalSection(&m_cs);
	CDialogEx::OnClose();
}
