#pragma once
#include <afxsock.h>

class CChatRoomClientDlg;

class CClientSoket :
    public CSocket
{
public:
   CClientSoket(void);
   CClientSoket(CChatRoomClientDlg* pMdlg);
   ~CClientSoket();

private:
    CChatRoomClientDlg* m_pMdlg;//主对话框的地址
    CString m_UserName;//用户名
    UINT m_MessFlag;//消息标志位 0：服务器回馈的消息 1：服务器发送的消息
    CStringArray m_OnUserArray;//存放上线用户的名字
    CStringArray m_CtoCMess;//存放CtoC的消息
    UINT  OnUserNum;//上线人数
public:
    
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);
    void GetUserName();
    CString DisposeMessages(TCHAR* recvBuf);
    virtual void OnClose(int nErrorCode);
    int DivCString(const CString strLine, char split, CStringArray& strArray);
    int Div1CString(const CString strLine, char split, CStringArray& strArray);//找到第一个就结束
    bool SplitCStringByLastAsterisk(const CString& str, CString& firstPart, CString& secondPart);
};

