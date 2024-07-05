
// ChatRoomClient.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号

#include <list>
using namespace std;

// CChatRoomClientApp:
// 有关此类的实现，请参阅 ChatRoomClient.cpp
//

class CChatRoomClientApp : public CWinApp
{
public:
	CChatRoomClientApp();
	list<CString*> m_UserList;//存放用户名字的链表
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CChatRoomClientApp theApp;
