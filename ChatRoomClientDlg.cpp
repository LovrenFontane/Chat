
// ChatRoomClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChatRoomClient.h"
#include "ChatRoomClientDlg.h"
#include "afxdialogex.h"
#include "CClientSoket.h"
#include "MySoloDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatRoomClientDlg 对话框



CChatRoomClientDlg::CChatRoomClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOMCLIENT_DIALOG, pParent)
	, m_port(6666)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_LIST_MESS, m_ListMess);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_UserName);
	DDX_Control(pDX, IDC_EDIT_MESS, m_SendMess);
	DDX_Control(pDX, IDC_LIST_ONLINE, m_OnlineUser);
}

BEGIN_MESSAGE_MAP(CChatRoomClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CChatRoomClientDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUT_CONN, &CChatRoomClientDlg::OnBnClickedButConn)
	ON_BN_CLICKED(IDC_BUT_DISCONN, &CChatRoomClientDlg::OnBnClickedButDisconn)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ONLINE, &CChatRoomClientDlg::OnNMClickList1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUT_SEND, &CChatRoomClientDlg::OnBnClickedButSend)
	ON_BN_CLICKED(IDC_BUT_SAVE, &CChatRoomClientDlg::OnBnClickedButSave)
	ON_BN_CLICKED(IDC_BUT_IMG, &CChatRoomClientDlg::OnBnClickedButImg)
END_MESSAGE_MAP()


// CChatRoomClientDlg 消息处理程序

BOOL CChatRoomClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitListCtr();

	m_pChatDlg = new MySoloDlg;
	m_pChatDlg->Create(IDD_DIALOGSOLO);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatRoomClientDlg::OnPaint()
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
HCURSOR CChatRoomClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//保存对话框及其所有子窗体的Rect区域  
void CChatRoomClientDlg::GetInitRect()
{
	CRect rect;
	GetWindowRect(&rect);
	m_listRect.AddTail(rect);//对话框的区域  

	CWnd* pWnd = GetWindow(GW_CHILD);//获取子窗体  
	while (pWnd)
	{
		pWnd->GetWindowRect(rect);//子窗体的区域  
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect成员变量  
		pWnd = pWnd->GetNextWindow();//取下一个子窗体  
	}
}

void CChatRoomClientDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CChatRoomClientDlg::OnBnClickedButConn()
{
	UpdateData(TRUE);
	BYTE a, b, c, d;
	m_ip.GetAddress(a, b, c, d);
	CString ip;
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);


	//创建对象
	m_pClientSocket = new CClientSoket(this);
	//创建套接字
	CString strMsg;

	if (FALSE == m_pClientSocket->Create())
	{
		strMsg.Format(_T("创建套接字失败!!error:%d"), GetLastError());
		MessageBox(strMsg, _T("温馨提示！"), MB_OK);
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		//关闭对话框
		EndDialog(IDOK);
	}

	//连接服务器
	if (FALSE == m_pClientSocket->Connect(ip, m_port))
	{
		strMsg.Format(_T("连接服务器失败失败!!error:%d"), GetLastError());
		MessageBox(strMsg, _T("温馨提示！"), MB_OK);
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		//关闭对话框
		EndDialog(IDOK);
	}


	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_CONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_DISCONN)->EnableWindow(TRUE);
}


void CChatRoomClientDlg::OnBnClickedButDisconn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pClientSocket->Close();//关闭套接字

	//CString ShowMsg;
	//ShowMsg = _T("与服务器断开！");
	//m_ListMess.AddString(ShowMsg);

	ResetControl();
}


void CChatRoomClientDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CChatRoomClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_listRect.GetCount() > 0)
	{
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = m_listRect.GetHeadPosition();//第一个保存的是对话框的Rect  

		CRect dlgSaved;
		dlgSaved = m_listRect.GetNext(pos);
		ScreenToClient(dlgNow);

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();//根据当前和之前保存的对话框的宽高求比例  
		float y = dlgNow.Height() * 1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);

		CRect childSaved;

		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			childSaved = m_listRect.GetNext(pos);//依次获取子窗体的Rect  
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left) * x;//根据比例调整控件上下左右距离对话框的距离  
			childSaved.right = dlgNow.right + (childSaved.right - dlgSaved.right) * x;
			childSaved.top = dlgNow.top + (childSaved.top - dlgSaved.top) * y;
			childSaved.bottom = dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom) * y;
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved);
			pWnd = pWnd->GetNextWindow();
		}

	}

}


void CChatRoomClientDlg::OnBnClickedButSend()
{
	// TODO: 在此添加控件通知处理程序代码
	//向服务器发送消息
	TCHAR* msg;
	CString strMsg;

	m_SendMess.GetWindowText(strMsg);
	strMsg = _T("b") + strMsg;

	msg = strMsg.GetBuffer(strMsg.GetLength());
	m_pClientSocket->Send(msg, strlen(msg), 0); //给服务器发送信息

	//清空编辑框
	m_SendMess.SetSel(0, -1);
	m_SendMess.ReplaceSel(_T(""));
	UpdateData(FALSE);
}


void CChatRoomClientDlg::ResetControl()
{
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUT_CONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUT_DISCONN)->EnableWindow(FALSE);

	m_OnlineUser.DeleteAllItems();//清空在线用户列表
}

void  CChatRoomClientDlg::InitListCtr()
{
	//UpdateData(FALSE);

	DWORD dwStyle = m_OnlineUser.GetExtendedStyle(); //显示网格线
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_OnlineUser.SetExtendedStyle(dwStyle);

	m_OnlineUser.InsertColumn(0, _T("在线用户"), LVCFMT_CENTER,150);
	//m_OnlineUser.InsertItem(0, _T("离线"));
	 //m_OnlineUser.InsertItem(m_OnlineUser.GetItemCount(), _T("RecvMsg"));
	
}

//单击获取在线列表中的信息
void CChatRoomClientDlg::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strName;
	int row;
	//首先得到点击的位置
	POSITION pos = m_OnlineUser.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("请至少选择一项"), _T("提示"), MB_ICONEXCLAMATION);
		return;
	}
	//得到行号，通过POSITION转化
	row = (int)m_OnlineUser.GetNextSelectedItem(pos);
	//获取第row第0列的内容，并保存到str中
	m_Receiver = m_OnlineUser.GetItemText(row, 0);

	m_UserName.GetWindowTextA(strName);
	if (m_Receiver == strName)
	{
		MessageBox(_T("不要选择自己"), _T("提示"), MB_ICONEXCLAMATION);
		return;
	}
	//弹出chat对话框
	m_pChatDlg->ShowWindow(SW_SHOWNORMAL);

	m_pChatDlg->m_ChatName.SetWindowText(m_Receiver);
	//SetDlgItemText(IDC_EDIT_AIM, m_Receiver);//显示接收人的信息
}


void CChatRoomClientDlg::OnBnClickedButSave()
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
	int n= m_ListMess.GetCount();
	for (int i = 0; i < n; i++)
	{
		m_ListMess.GetText(i, str);
		fprintf(fp, "%s\n", str);
	}
	fclose(fp);
	str.Format("聊天记录保存至%s", filename);
	MessageBox(str);
}


void CChatRoomClientDlg::OnBnClickedButImg()
{
	TCHAR* msg;
	CString strMsg;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//TRUE打开文件 FLASE另存为
		TEXT("JPG文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|BMP文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||"));
	if (IDCANCEL == dlg.DoModal())
	{
		return;
	}

	//图片路径获取
	CString strPath = dlg.GetPathName();
	strMsg.Format("%s", PathFindFileName(strPath));
	strMsg = _T("d") + strMsg;
	msg = strMsg.GetBuffer(strMsg.GetLength());
	
	m_pClientSocket->Send(msg, strlen(msg), 0); //给服务器发送信息

	char buf[1024] = { 0 };
	FILE* fp = fopen(strPath, "rb");
	//打开全路径文件 发送文件大小
	fseek(fp, 0, SEEK_END);
	int allbytes = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	Sleep(100);
	m_pClientSocket->Send((const char*)&allbytes, sizeof(allbytes));
	Sleep(100);
	
	//将文件内容读取出来发送过去
	int sum = 0;
	int bytes;
	while ((bytes = fread(buf, 1, 1024, fp)) > 0)
	{
		m_pClientSocket->Send(buf, bytes);
		sum += bytes;
	}

	fclose(fp);
}
