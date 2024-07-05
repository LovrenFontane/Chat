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
    CChatRoomClientDlg* m_pMdlg;//���Ի���ĵ�ַ
    CString m_UserName;//�û���
    UINT m_MessFlag;//��Ϣ��־λ 0����������������Ϣ 1�����������͵���Ϣ
    CStringArray m_OnUserArray;//��������û�������
    CStringArray m_CtoCMess;//���CtoC����Ϣ
    UINT  OnUserNum;//��������
public:
    
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);
    void GetUserName();
    CString DisposeMessages(TCHAR* recvBuf);
    virtual void OnClose(int nErrorCode);
    int DivCString(const CString strLine, char split, CStringArray& strArray);
    int Div1CString(const CString strLine, char split, CStringArray& strArray);//�ҵ���һ���ͽ���
    bool SplitCStringByLastAsterisk(const CString& str, CString& firstPart, CString& secondPart);
};

