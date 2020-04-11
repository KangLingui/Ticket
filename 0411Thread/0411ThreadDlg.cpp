
// 0411ThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "0411Thread.h"
#include "0411ThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMy0411ThreadDlg 对话框



CMy0411ThreadDlg::CMy0411ThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy0411ThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_shuliang = 100;
	InitializeCriticalSection(&m_cs);//关键段
	m_Mutex = CreateMutex(0,0,0);//互斥量
	m_Event = CreateEvent(0,//安全属性
		false,//事件false 创建自动事件  true  人工事件
		true,//置位有信号
		NULL);
	 m_hsemphore = CreateSemaphore(0,
		 1,//初始化个数
		 10,//信号量最大的个数
		 0
		 
		 );//信号量

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


// CMy0411ThreadDlg 消息处理程序

BOOL CMy0411ThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy0411ThreadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy0411ThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//线程同步  ----解决线程并发的问题
//1.原子访问 ---- 同一时刻只允许一个线程访问资源（变量）（实现方式：Inclock）
//volatile  防止编译优化，对特殊地址的稳定访问
//2.关键段（用户模式下的线程同步）-------有起始和终止  中间的代码只允许一个线程跑
//运行（进不去）调到-阻塞-（入口可以进去）就绪-运行2.1（直接阻塞）
//等一会（能进去就进去，进不去到阻塞态）2.2旋转锁
//2.3异步处理（针对对个入口）(试图进去，进不去继续找其他入口)
//3(内核对象) 
		//3.1互斥量--同一时刻允许一个线程访问代码段
		// 区别：1互斥量可以跨进程；2灵活安全；3效率：关键段高，内核模式需要切换 4互斥量可以设置初始拥有权
		//3.2  事件（内核对象）--互斥量--同一时刻允许一个线程访问代码段
		//    人工事件，自动事件
		//3.3信号量 --------有个数限制




//线程函数
DWORD WINAPI ThreadPro(LPVOID lppar)
{

//票数减减
	CMy0411ThreadDlg *pthis = (CMy0411ThreadDlg*)lppar;
	CString str;
	while (1)
	{
		if(pthis->m_shuliang==0)
		{
			str.Format(_T("%d结束了"),GetCurrentThreadId());
			pthis->m_listbox.AddString(str);
		break;
		}
			


		Sleep(10);
			WaitForSingleObject(pthis->m_hsemphore,INFINITE);//信号量--入口
					//WaitForSingleObject(pthis->m_Event,INFINITE);//事件--入口
							//WaitForSingleObject(pthis->m_Mutex,INFINITE);//互斥量--入口
									//EnterCriticalSection(&pthis->m_cs);//关键段--起始
		
												if(pthis->m_shuliang==0)
												{
														str.Format(_T("%d走了"),GetCurrentThreadId());
														pthis->m_listbox.AddString(str);
														//LeaveCriticalSection(&pthis->m_cs);//关键段--结束
														//ReleaseMutex(pthis->m_Mutex);//互斥量--出口
														//SetEvent(pthis->m_Event);//事件--置位无信号
														ReleaseSemaphore(pthis->m_hsemphore,1,0);//信号量
														break;
												}
												str.Format(_T("%d正在买第%d张车票"),GetCurrentThreadId(),pthis->m_shuliang);
												//InterlockedDecrement(&pthis->m_shuliang);//原子访问
												pthis->m_shuliang--;
									//LeaveCriticalSection(&pthis->m_cs);//关键段--结束
							//ReleaseMutex(pthis->m_Mutex);//互斥量--出口
					//SetEvent(pthis->m_Event);//事件--置位无信号
			ReleaseSemaphore(pthis->m_hsemphore,1,0);//信号量--出口

		pthis->m_listbox.AddString(str);
	}
    return 0;
}




void CMy0411ThreadDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//创建10个线程
	

	for(int i = 0;i<10 ;i++)
	{
	CreateThread(NULL,0,&ThreadPro,this,0,0);

	}
}


void CMy0411ThreadDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DeleteCriticalSection(&m_cs);
	CDialogEx::OnClose();
}
