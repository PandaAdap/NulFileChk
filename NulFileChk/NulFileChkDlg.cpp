
// NulFileChkDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NulFileChk.h"
#include "NulFileChkDlg.h"
#include "afxdialogex.h"

#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT Thread_Ergodic(LPVOID lpParam);
UINT Thread_NulFileChk(LPVOID lpParam);

void FindFileInDir(CString rootDir, CNulFileChkDlg* obj);

CString ServerPath = L"";

std::vector<CString> Queue_NulFile;

bool working = false;
bool is_Ergodic = false;
bool is_NulFileChk = false;

int index_Max = 500;
int index_Ergodic = 0;
int index_NulFileChk = 0;

int counter_file = 0;
int counter_chked = 0;
int counter_nulfile = 0;

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


// CNulFileChkDlg 对话框



CNulFileChkDlg::CNulFileChkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NULFILECHK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNulFileChkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FileReport, m_filereport);
}

BEGIN_MESSAGE_MAP(CNulFileChkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Btn_JarOpen, &CNulFileChkDlg::OnBnClickedBtnJaropen)
	ON_BN_CLICKED(IDC_Btn_Start, &CNulFileChkDlg::OnBnClickedBtnStart)
	ON_NOTIFY(NM_RCLICK, IDC_FileReport, &CNulFileChkDlg::OnNMRClickFilereport)
	ON_COMMAND(ID_RCMENU_Delete, &CNulFileChkDlg::OnRcmenuDelete)
	ON_COMMAND(ID_RCMENU_Check, &CNulFileChkDlg::OnRcmenuCheck)
	ON_BN_CLICKED(IDC_Btn_Clean, &CNulFileChkDlg::OnBnClickedBtnClean)
END_MESSAGE_MAP()


// CNulFileChkDlg 消息处理程序

BOOL CNulFileChkDlg::OnInitDialog()
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

	setlocale(LC_ALL, "chs");
	// TODO: 在此添加额外的初始化代码
	CRect rect;
	
	m_filereport.GetClientRect(&rect);
	m_filereport.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_filereport.SetExtendedStyle(m_filereport.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_filereport.InsertColumn(0, _T("Item"), LVCFMT_LEFT, rect.Width() - 5);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNulFileChkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNulFileChkDlg::OnPaint()
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
HCURSOR CNulFileChkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNulFileChkDlg::OnBnClickedBtnJaropen()
{
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, L"Minecraft Core|*.jar||", NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		SetDlgItemText(IDC_Static_JarFile, openFileDlg.GetPathName());
		ServerPath = openFileDlg.GetPathName().Mid(0, openFileDlg.GetPathName().ReverseFind('\\'));
	}
}

UINT Thread_Ergodic(LPVOID lpParam)
{
	CNulFileChkDlg* obj = (CNulFileChkDlg*)lpParam;

	FindFileInDir(ServerPath, obj);

	return 0;
}

UINT Thread_NulFileChk(LPVOID lpParam)
{
	CNulFileChkDlg* obj = (CNulFileChkDlg*)lpParam;

	CString str;
	Sleep(100);
	while (1)
	{
		if (index_Ergodic == index_Max)
		{
			Sleep(20);
		}

		index_NulFileChk = 0;
		for (int i = 0; i < index_Max; i++)
		{
			if (working == false)
			{
				goto end1;
			}
			while (i > Queue_NulFile.size() - 1)
			{
				if (is_Ergodic == false)
				{
					goto end1;
				}
				Sleep(100);
			}
			CFile readfile;
			readfile.Open(Queue_NulFile[i], CFile::modeRead | CFile::typeBinary);

			UINT len;
			len = readfile.GetLength();	

			BYTE* ucharbuffer = new BYTE[len]();
			readfile.Read(ucharbuffer, len);
			readfile.Close();

			if (len > 0)//NUL File
			{
				bool isnulfile = true;
				for (int bufchk = 0; bufchk < len; bufchk++)
				{
					if (ucharbuffer[bufchk] != 0x00)
					{ 
						isnulfile = false;
						break;
					}
				}
				if (isnulfile == true)
				{
					counter_nulfile++;
					str.Format(L"Invalid: %d", counter_nulfile);
					obj->SetDlgItemText(IDC_Static_NulCounter, str);

					obj->m_filereport.InsertItem(0, Queue_NulFile[i]);
				}
			}

			delete[] ucharbuffer;

			index_NulFileChk++;
			counter_chked++;

			str.Format(L"Checked: %d", counter_chked);
			obj->SetDlgItemText(IDC_Static_ChkCounter, str);
		}
	}

end1:

	is_NulFileChk = false;

	Queue_NulFile.clear();

	index_Ergodic = 0;
	index_NulFileChk = 0;

	counter_file = 0;
	counter_chked = 0;
	counter_nulfile = 0;

	if(working == true)
		obj->OnBnClickedBtnStart();

	return 0;
}

void CNulFileChkDlg::OnBnClickedBtnStart()
{
	if (working == false)
	{
		if (!PathIsDirectory(ServerPath))
		{
			return;
		}
		working = true;

		Queue_NulFile.clear();

		m_filereport.DeleteAllItems();

		index_Ergodic = 0;
		index_NulFileChk = 0;

		counter_file = 0;
		counter_chked = 0;
		counter_nulfile = 0;

		GetDlgItem(IDC_Btn_JarOpen)->EnableWindow(FALSE);
		GetDlgItem(IDC_Btn_Clean)->EnableWindow(FALSE);
		GetDlgItem(IDC_Btn_Start)->SetWindowText(L"Stop");

		is_Ergodic = true;
		is_NulFileChk = false;

		AfxBeginThread(Thread_Ergodic, this);
		AfxBeginThread(Thread_NulFileChk, this);

	}
	else if (working == true)
	{
		working = false;
		GetDlgItem(IDC_Btn_JarOpen)->EnableWindow(TRUE);
		GetDlgItem(IDC_Btn_Clean)->EnableWindow(TRUE);
		GetDlgItem(IDC_Btn_Start)->SetWindowText(L"Start");
	}
}


void FindFileInDir(CString rootDir, CNulFileChkDlg* obj)
{
	// 查找当前路径下的所有文件夹和文件
	CString strDir = rootDir;
	strDir += "\\*.*";

	// 遍历得到所有子文件夹名
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDir);

	while (bWorking)
	{
		if (working == false)
		{
			goto end;
		}

		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && "." != finder.GetFileName() && ".." != finder.GetFileName())
		{
			FindFileInDir(finder.GetFilePath(), obj);
		}
		else if (!finder.IsDirectory())
		{
			Queue_NulFile.push_back(finder.GetFilePath());
			counter_file++;
			index_Ergodic++;
			
			CString str;
			str.Format(L"File: %d", counter_file);
			obj->SetDlgItemText(IDC_Static_FileCounter, str);

			if (index_Ergodic == index_Max)
			{
				while (1)
				{
					Sleep(10);

					if (working == false)
					{
						goto end;
					}

					if (index_NulFileChk == index_Ergodic)
					{
						index_Ergodic = 0;
						Queue_NulFile.clear();
						break;
					}
				}
			}

		}
	}

end:
	finder.Close();
	is_Ergodic = false;
}

void CNulFileChkDlg::OnNMRClickFilereport(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CMenu menu, * popup;
	menu.LoadMenu(IDR_RCMenu);
	popup = menu.GetSubMenu(0);

	if (m_filereport.GetSelectedCount() <= 0)
	{
		popup->EnableMenuItem(ID_RCMENU_Delete, MF_GRAYED);
	}
	else
	{
		popup->EnableMenuItem(ID_RCMENU_Delete, MF_ENABLED);
	}

	CPoint point;
	ClientToScreen(&point);
	GetCursorPos(&point);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x + 1, point.y + 1, this);

	*pResult = 0;
}


void CNulFileChkDlg::OnRcmenuDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_filereport.GetFirstSelectedItemPosition();
	int line = (int)m_filereport.GetNextSelectedItem(pos);
	CString str = m_filereport.GetItemText(line, 0);

	if (MessageBox(L"Will delete this file forever!\r\nEnsure that you have already checked this file!\r\n\r\nFile: " + str, L"Delete?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		DeleteFile(str);
		m_filereport.DeleteItem(line);
	}
}


void CNulFileChkDlg::OnRcmenuCheck()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_filereport.GetFirstSelectedItemPosition();
	int line = (int)m_filereport.GetNextSelectedItem(pos);
	CString str = m_filereport.GetItemText(line, 0);

	ShellExecute(NULL, L"open", str, NULL, NULL, SW_SHOW);
}


void CNulFileChkDlg::OnBnClickedBtnClean()
{
	GetDlgItem(IDC_Btn_Clean)->EnableWindow(FALSE);
	if (MessageBox(L"Will delete all invalid files forever!\r\nEnsure that you have already checked the files!\r\n", L"Delete?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		int items = m_filereport.GetItemCount();
		for (int i = 0; i < items; i++)
		{
			CString str = m_filereport.GetItemText(0, 0);
			DeleteFile(str);
			m_filereport.DeleteItem(0);
		}
	}
	GetDlgItem(IDC_Btn_Clean)->EnableWindow(TRUE);
}
