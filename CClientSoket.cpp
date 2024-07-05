#include "pch.h"
#include "CClientSoket.h"
#include "ChatRoomClientDlg.h"
#include "MySoloDlg.h"


CClientSoket::CClientSoket()
{


}

 CClientSoket::CClientSoket(CChatRoomClientDlg* pMdlg)
{
	m_pMdlg = pMdlg;
}

 CClientSoket::~CClientSoket()
 {


 }

void CClientSoket::OnReceive(int nErrorCode)
{
	// �������ɿ�ܵ��ã�֪ͨ�׽��ֻ����������ݣ����Ե���Receive��Ա����ȡ��
	TCHAR recvBuf[512] = {0};
	int recvLen = this->Receive(recvBuf, sizeof(recvBuf),0);

	CString RecvMsg,ShowMsg,TimeMsg,SenderMsg;
	SYSTEMTIME t;

	RecvMsg = DisposeMessages(recvBuf);//��������Ϣ

	GetLocalTime(&t);
	TimeMsg.Format(_T("[%d:%d:%d]"), t.wHour, t.wMinute, t.wSecond);
	if (m_MessFlag == 0)//������������Ϣ
	{
		ShowMsg = TimeMsg + _T(" ") + RecvMsg;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}
	
	if (m_MessFlag == 1)//��������Ϣ
	{
		ShowMsg = TimeMsg + _T(" ��������") + RecvMsg;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}

	if (m_MessFlag == 2)//�����û�
	{
		CString  tempMsg;
		m_pMdlg->m_OnlineUser.DeleteAllItems();
		for (int m = 0; m < OnUserNum-1; m++)//���һ��Ϊ��
		{
			m_pMdlg->m_OnlineUser.InsertItem(m, m_OnUserArray.GetAt(m));//���һ�в�����
		}
		

	}

	if (m_MessFlag == 3)//�յ�˽����Ϣ
	{
		SenderMsg = m_CtoCMess.GetAt(0);
		m_pMdlg->m_pChatDlg->ShowWindow(SW_SHOWNORMAL);

		m_pMdlg->m_pChatDlg->m_ChatName.SetWindowText(SenderMsg);//������

		ShowMsg = TimeMsg + SenderMsg + _T(": ") + m_CtoCMess.GetAt(1);
		m_pMdlg->m_pChatDlg->m_ListMess.AddString(ShowMsg);

	}

	if (m_MessFlag == 4)//ͼƬ
	{
		CString user, img;
		SplitCStringByLastAsterisk(RecvMsg, user, img);

		CreateDirectory("img", NULL);
		CString strPath;
		strPath.Format("./img/%s", img);

		FILE* fp = fopen(strPath, "wb");
		int allbytes = 0;
		TCHAR buf[1024] = { 0 };
		Sleep(100);
		this->Receive((char*)&allbytes, sizeof(allbytes));
		Sleep(100);

		if (allbytes == 0)
		{
			fclose(fp);
			return;
		}

		int bytes, sum = 0;
		while ((bytes = this->Receive(buf, 1024)) > 0)
		{
			fwrite(buf, 1, bytes, fp);
			sum += bytes;
			if (sum >= allbytes)
			{
				break;
			}
		}

		fclose(fp);

		ShowMsg = TimeMsg + _T(" ") + user + _T("��") + img;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}

	if (m_MessFlag == 5)//˽��ͼƬ
	{
		CreateDirectory("img", NULL);
		CString strPath;
		strPath.Format("./img/%s", m_CtoCMess.GetAt(1));

		FILE* fp = fopen(strPath, "wb");
		int allbytes = 0;
		TCHAR buf[1024] = { 0 };
		Sleep(100);
		this->Receive((char*)&allbytes, sizeof(allbytes));
		Sleep(100);

		if (allbytes == 0)
		{
			fclose(fp);
			return;
		}

		int bytes, sum = 0;
		while ((bytes = this->Receive(buf, 1024)) > 0)
		{
			fwrite(buf, 1, bytes, fp);
			sum += bytes;
			if (sum >= allbytes)
			{
				break;
			}
		}

		fclose(fp);

		SenderMsg = m_CtoCMess.GetAt(0);
		m_pMdlg->m_pChatDlg->ShowWindow(SW_SHOWNORMAL);

		m_pMdlg->m_pChatDlg->m_ChatName.SetWindowText(SenderMsg);//������

		ShowMsg = TimeMsg + SenderMsg + _T(": ") + m_CtoCMess.GetAt(1);
		m_pMdlg->m_pChatDlg->m_ListMess.AddString(ShowMsg);


	}

	CSocket::OnReceive(nErrorCode);
}


void CClientSoket::OnSend(int nErrorCode)
{
	//���ӷ����������Լ�����Ϣ
	// ��ȡ�û���
	TCHAR* msg;
	GetUserName();
	m_UserName = _T("a") + m_UserName ;
	msg = m_UserName.GetBuffer(m_UserName.GetLength());
    Send(msg, strlen(msg),0); //��������������Ϣ

	CSocket::OnSend(nErrorCode);
}

//��ȡ�û���
void CClientSoket::GetUserName()
{
	m_pMdlg->m_UserName.GetWindowText(m_UserName);
}


//���ַ��������͵���Ϣ
CString CClientSoket::DisposeMessages(TCHAR* recvBuf)
{
	CString msg;

	msg.Format(_T("%s"), recvBuf);//TCHAR => CString
	if (0 == msg.FindOneOf(_T("-")))//������Ϣ
	{
		msg.Delete(0);
		m_MessFlag = 0;
	}

	else if (0 == msg.FindOneOf(_T("+")))//������Ϣ
	{
		msg.Delete(0);
		m_MessFlag = 1;
	}

	else if (0 == msg.FindOneOf(_T("=")))//�����û���
	{
		msg.Delete(0);
		OnUserNum = DivCString(msg,'=', m_OnUserArray);
		m_MessFlag = 2;
	}

	else if (0 == msg.FindOneOf(_T("*")))//˽����Ϣ
	{
		msg.Delete(0);
		Div1CString(msg, '%', m_CtoCMess);
		m_MessFlag = 3;
	}

	else if (0 == msg.FindOneOf(_T("d")))//����ͼƬ
	{
		msg.Delete(0);
		m_MessFlag = 4;
	}

	else if (0 == msg.FindOneOf(_T("e")))//˽��ͼƬ
	{
		msg.Delete(0);
		Div1CString(msg, '%', m_CtoCMess);
		m_MessFlag = 5;
	}

	return(msg);
}

void CClientSoket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CString ShowMsg;
	ShowMsg = _T("�������ѶϿ���");
	m_pMdlg->m_ListMess.AddString(ShowMsg);

	m_pMdlg->ResetControl();

	this->Close();
	CSocket::OnClose(nErrorCode);
}

//***********************************
//�ַ��ָ��
//***********************************
int CClientSoket::DivCString(const CString strLine, char split, CStringArray& strArray)
{
	strArray.RemoveAll();//�Դ��������
	CString temp = strLine;
	int tag = 0;
	while (1)
	{
		tag = temp.Find(split);
		if (tag >= 0)
		{
			strArray.Add(temp.Left(tag));
			temp = temp.Right(temp.GetLength() - tag - 1);
		}
		else { break; }
	}
	strArray.Add(temp);
	return strArray.GetSize();
}


int CClientSoket::Div1CString(const CString strLine, char split, CStringArray& strArray)
{
	strArray.RemoveAll();//�Դ��������
	CString temp = strLine;
	int tag = 0;

		tag = temp.Find(split);
		if (tag >= 0)
		{
			strArray.Add(temp.Left(tag));
			temp = temp.Right(temp.GetLength() - tag - 1);
		}

	strArray.Add(temp);
	return strArray.GetSize();
}


bool CClientSoket::SplitCStringByLastAsterisk(const CString& str, CString& firstPart, CString& secondPart)
{
	int pos = str.ReverseFind('*');
	if (pos == -1)
	{
		// û���ҵ�'*'���޷��ָ�����趨��������Ϊԭʼ�ַ����Ϳ��ַ���  
		firstPart = str;
		secondPart.Empty();
		return false; // ���������ѡ���׳�һ���쳣�򷵻�ĳ�ִ������  
	}

	firstPart = str.Left(pos); // ��ȡ���һ��'*'֮ǰ�������ַ�  
	secondPart = str.Mid(pos + 1); // ��ȡ���һ��'*'֮��������ַ�  

	return true; // �ָ�ɹ�  
}