
// �ಥ�շ���������.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C�ಥ�շ���������App: 
// �йش����ʵ�֣������ �ಥ�շ���������.cpp
//

class C�ಥ�շ���������App : public CWinApp
{
public:
	C�ಥ�շ���������App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C�ಥ�շ���������App theApp;