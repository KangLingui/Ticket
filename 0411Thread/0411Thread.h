
// 0411Thread.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy0411ThreadApp:
// �йش����ʵ�֣������ 0411Thread.cpp
//

class CMy0411ThreadApp : public CWinApp
{
public:
	CMy0411ThreadApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy0411ThreadApp theApp;