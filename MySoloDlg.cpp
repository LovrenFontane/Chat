// MySoloDlg.cpp: 实现文件
//

#include "pch.h"
#include "ChatRoomClient.h"
#include "MySoloDlg.h"
#include "afxdialogex.h"
#include "ChatRoomClientDlg.h"


// MySoloDlg 对话框

IMPLEMENT_DYNAMIC(MySoloDlg, CDialogEx)

MySoloDlg::MySoloDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGSOLO, pParent)
{

}

MySoloDlg::~MySoloDlg()
{
}

void MySoloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UserName, m_ChatName);
	DDX_Control(pDX, IDC_LIST_MESS, m_ListMess);
	DDX_Control(pDX, IDC_EDIT_MESS, m_EditMess);
}


BEGIN_MESSAGE_MAP(MySoloDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTSEND, &MySoloDlg::OnBnClickedButsend)
	ON_BN_CLICKED(IDC_BUT_SAVE, &MySoloDlg::OnBnClickedButSave)
	ON_BN_CLICKED(IDC_BUT_IMG, &MySoloDlg::OnBnClickedButImg)
END_MESSAGE_MAP()


// MySoloDlg 消息处理程序


void MySoloDlg::OnBnClickedButsend()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR* msg;
	CString strMsg,RecNMsg,ShowMsg,OwnMsg,TimeMsg;

	m_EditMess.GetWindowText(ShowMsg);//待发送信息
	m_ChatName.GetWindowTextA(RecNMsg);//接收人
	strMsg = _T("c") + RecNMsg + _T("%") + ShowMsg;
	msg = strMsg.GetBuffer(strMsg.GetLength());

	CChatRoomClientDlg* pMDlg = (CChatRoomClientDlg*)AfxGetMainWnd();
	pMDlg->m_pClientSocket->Send(msg, strlen(msg), 0); //给服务器发送信息
	pMDlg->m_UserName.GetWindowTextA(OwnMsg);

	SYSTEMTIME t;
	GetLocalTime(&t);
	TimeMsg.Format(_T("[%d:%d:%d]"), t.wHour, t.wMinute, t.wSecond);

	ShowMsg = TimeMsg+OwnMsg + _T("：") + ShowMsg;
	m_ListMess.AddString(ShowMsg);
	//清空编辑框
	m_EditMess.SetSel(0, -1);
	m_EditMess.ReplaceSel(_T(""));
	UpdateData(FALSE);
}

void MySoloDlg::OnBnClickedButSave()
{
	CreateDirectory("ChatData", NULL);
	CString filename;
	srand((unsigned int)time(0));
	// 获取当前时间戳
	time_t now = time(NULL);

	// 转换为本地时间
	struct tm* local = localtime(&now);

	// 获取年、月、日、时、分、秒
	int year = local->tm_year + 1900;
	int month = local->tm_mon + 1;
	int day = local->tm_mday;
	int hour = local->tm_hour;
	int minute = local->tm_min;
	int second = local->tm_sec;

	// 输出时间
	filename.Format("ChatData/%d%02d%02d%02d%02d%02d_%d%d%d%d.txt",
		year, month, day, hour, minute, second,
		rand() % 10, rand() % 10, rand() % 10, rand() % 10);

	FILE* fp = fopen(filename, "w");
	CString str;
	int n = m_ListMess.GetCount();
	for (int i = 0; i < n; i++)
	{
		m_ListMess.GetText(i, str);
		fprintf(fp, "%s\n", str);
	}
	fclose(fp);
	str.Format("聊天记录保存至%s", filename);
	MessageBox(str);
}


void MySoloDlg::OnBnClickedButImg()
{
	TCHAR* msg;
	CString strMsg, RecNMsg, OwnMsg, TimeMsg, ShowMsg;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//TRUE打开文件 FLASE另存为
		TEXT("JPG文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|BMP文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||"));
	if (IDCANCEL == dlg.DoModal())
	{
		return;
	}

	//图片路径获取
	CString strPath = dlg.GetPathName();
	strMsg.Format("%s", PathFindFileName(strPath));
	m_ChatName.GetWindowTextA(RecNMsg);//接收人
	strMsg = _T("e") + RecNMsg + _T("%") + strMsg;
	msg = strMsg.GetBuffer(strMsg.GetLength());

	CChatRoomClientDlg* pMDlg = (CChatRoomClientDlg*)AfxGetMainWnd();
	pMDlg->m_pClientSocket->Send(msg, strlen(msg), 0); //给服务器发送信息

	char buf[1024] = { 0 };
	FILE* fp = fopen(strPath, "rb");
	//打开全路径文件 发送文件大小
	fseek(fp, 0, SEEK_END);
	int allbytes = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	Sleep(100);
	pMDlg->m_pClientSocket->Send((const char*)&allbytes, sizeof(allbytes));
	Sleep(100);

	//将文件内容读取出来发送过去
	int sum = 0;
	int bytes;
	while ((bytes = fread(buf, 1, 1024, fp)) > 0)
	{
		pMDlg->m_pClientSocket->Send(buf, bytes);
		sum += bytes;
	}

	fclose(fp);

	pMDlg->m_UserName.GetWindowTextA(OwnMsg);
	SYSTEMTIME t;
	GetLocalTime(&t);
	TimeMsg.Format(_T("[%d:%d:%d]"), t.wHour, t.wMinute, t.wSecond);
	ShowMsg = TimeMsg + OwnMsg + _T("：") + strPath;
	m_ListMess.AddString(ShowMsg);
}
