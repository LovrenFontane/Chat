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
	// 本函数由框架调用，通知套接字缓冲中有数据，可以调用Receive成员函数取出
	TCHAR recvBuf[512] = {0};
	int recvLen = this->Receive(recvBuf, sizeof(recvBuf),0);

	CString RecvMsg,ShowMsg,TimeMsg,SenderMsg;
	SYSTEMTIME t;

	RecvMsg = DisposeMessages(recvBuf);//处理后的消息

	GetLocalTime(&t);
	TimeMsg.Format(_T("[%d:%d:%d]"), t.wHour, t.wMinute, t.wSecond);
	if (m_MessFlag == 0)//服务器回馈消息
	{
		ShowMsg = TimeMsg + _T(" ") + RecvMsg;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}
	
	if (m_MessFlag == 1)//服务器消息
	{
		ShowMsg = TimeMsg + _T(" 服务器：") + RecvMsg;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}

	if (m_MessFlag == 2)//上线用户
	{
		CString  tempMsg;
		m_pMdlg->m_OnlineUser.DeleteAllItems();
		for (int m = 0; m < OnUserNum-1; m++)//最后一个为空
		{
			m_pMdlg->m_OnlineUser.InsertItem(m, m_OnUserArray.GetAt(m));//最后一行插入行
		}
		

	}

	if (m_MessFlag == 3)//收到私聊消息
	{
		SenderMsg = m_CtoCMess.GetAt(0);
		m_pMdlg->m_pChatDlg->ShowWindow(SW_SHOWNORMAL);

		m_pMdlg->m_pChatDlg->m_ChatName.SetWindowText(SenderMsg);//发送者

		ShowMsg = TimeMsg + SenderMsg + _T(": ") + m_CtoCMess.GetAt(1);
		m_pMdlg->m_pChatDlg->m_ListMess.AddString(ShowMsg);

	}

	if (m_MessFlag == 4)//图片
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

		ShowMsg = TimeMsg + _T(" ") + user + _T("：") + img;
		m_pMdlg->m_ListMess.AddString(ShowMsg);
	}

	if (m_MessFlag == 5)//私聊图片
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

		m_pMdlg->m_pChatDlg->m_ChatName.SetWindowText(SenderMsg);//发送者

		ShowMsg = TimeMsg + SenderMsg + _T(": ") + m_CtoCMess.GetAt(1);
		m_pMdlg->m_pChatDlg->m_ListMess.AddString(ShowMsg);


	}

	CSocket::OnReceive(nErrorCode);
}


void CClientSoket::OnSend(int nErrorCode)
{
	//连接服务器后发送自己的信息
	// 获取用户名
	TCHAR* msg;
	GetUserName();
	m_UserName = _T("a") + m_UserName ;
	msg = m_UserName.GetBuffer(m_UserName.GetLength());
    Send(msg, strlen(msg),0); //给服务器发送信息

	CSocket::OnSend(nErrorCode);
}

//获取用户名
void CClientSoket::GetUserName()
{
	m_pMdlg->m_UserName.GetWindowText(m_UserName);
}


//区分服务器发送的信息
CString CClientSoket::DisposeMessages(TCHAR* recvBuf)
{
	CString msg;

	msg.Format(_T("%s"), recvBuf);//TCHAR => CString
	if (0 == msg.FindOneOf(_T("-")))//回馈消息
	{
		msg.Delete(0);
		m_MessFlag = 0;
	}

	else if (0 == msg.FindOneOf(_T("+")))//常规消息
	{
		msg.Delete(0);
		m_MessFlag = 1;
	}

	else if (0 == msg.FindOneOf(_T("=")))//上线用户名
	{
		msg.Delete(0);
		OnUserNum = DivCString(msg,'=', m_OnUserArray);
		m_MessFlag = 2;
	}

	else if (0 == msg.FindOneOf(_T("*")))//私聊消息
	{
		msg.Delete(0);
		Div1CString(msg, '%', m_CtoCMess);
		m_MessFlag = 3;
	}

	else if (0 == msg.FindOneOf(_T("d")))//回馈图片
	{
		msg.Delete(0);
		m_MessFlag = 4;
	}

	else if (0 == msg.FindOneOf(_T("e")))//私聊图片
	{
		msg.Delete(0);
		Div1CString(msg, '%', m_CtoCMess);
		m_MessFlag = 5;
	}

	return(msg);
}

void CClientSoket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString ShowMsg;
	ShowMsg = _T("服务器已断开！");
	m_pMdlg->m_ListMess.AddString(ShowMsg);

	m_pMdlg->ResetControl();

	this->Close();
	CSocket::OnClose(nErrorCode);
}

//***********************************
//字符分割函数
//***********************************
int CClientSoket::DivCString(const CString strLine, char split, CStringArray& strArray)
{
	strArray.RemoveAll();//自带清空属性
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
	strArray.RemoveAll();//自带清空属性
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
		// 没有找到'*'，无法分割，可以设定两个参数为原始字符串和空字符串  
		firstPart = str;
		secondPart.Empty();
		return false; // 或者你可以选择抛出一个异常或返回某种错误代码  
	}

	firstPart = str.Left(pos); // 提取最后一个'*'之前的所有字符  
	secondPart = str.Mid(pos + 1); // 提取最后一个'*'之后的所有字符  

	return true; // 分割成功  
}