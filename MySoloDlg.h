#pragma once


// MySoloDlg 对话框

class MySoloDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MySoloDlg)

public:
	MySoloDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MySoloDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGSOLO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 聊天人的姓名
	CStatic m_ChatName;
	// 显示聊天记录
	CListBox m_ListMess;
	// 待发送消息
	CEdit m_EditMess;
	afx_msg void OnBnClickedButsend();
	
	afx_msg void OnBnClickedButSave();
	afx_msg void OnBnClickedButImg();
};
