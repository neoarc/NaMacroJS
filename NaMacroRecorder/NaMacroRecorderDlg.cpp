
// NaMacroRecorderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "NaMacroRecorder.h"
#include "NaMacroRecorderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CNaMacroRecorderDlg 대화 상자



CNaMacroRecorderDlg::CNaMacroRecorderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAMACRORECORDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRecording = FALSE;
}

void CNaMacroRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaMacroRecorderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BTN_REC, &CNaMacroRecorderDlg::OnBnClickedBtnRec)
	ON_BN_CLICKED(IDC_BTN_STOP, &CNaMacroRecorderDlg::OnBnClickedBtnStop)
	ON_WM_INPUT()
END_MESSAGE_MAP()


// CNaMacroRecorderDlg 메시지 처리기

BOOL CNaMacroRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// Custom
	RegisterHotKey(m_hWnd, 0, 0, VK_F7);
	RegisterHotKey(m_hWnd, 1, 0, VK_F8);
	((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->SetCheck(TRUE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CNaMacroRecorderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNaMacroRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNaMacroRecorderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaMacroRecorderDlg::OnClose()
{
	// Custom
	UnregisterHotKey(m_hWnd, 0);
	UnregisterHotKey(m_hWnd, 1);

	CDialogEx::OnClose();
}


void CNaMacroRecorderDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	TRACE(_T("OnHotKey: %d\n"), nHotKeyId);
	switch(nHotKeyId) 
	{
		case 0:
			OnBnClickedBtnRec();
			break;
		case 1:
			OnBnClickedBtnStop();
			break;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CNaMacroRecorderDlg::OnBnClickedBtnRec()
{
	RecordStart();
}


void CNaMacroRecorderDlg::OnBnClickedBtnStop()
{
	RecordStop();
}

void CNaMacroRecorderDlg::RecordStart()
{
	if (m_bRecording)
		return;

	TRACE(L"Start Recording.\n");

	RAWINPUTDEVICE rawInputDev[2];
	ZeroMemory(rawInputDev, sizeof(RAWINPUTDEVICE) * 2);

	// key
	rawInputDev[0].usUsagePage = 0x01;
	rawInputDev[0].usUsage = 0x06;
	rawInputDev[0].dwFlags = RIDEV_INPUTSINK;
	rawInputDev[0].hwndTarget = m_hWnd;

	// mouse
	rawInputDev[1].usUsagePage = 0x01;
	rawInputDev[1].usUsage = 0x02;
	rawInputDev[1].dwFlags = RIDEV_INPUTSINK;
	rawInputDev[1].hwndTarget = m_hWnd;

	if (RegisterRawInputDevices(rawInputDev, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		/*
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		MessageBox(str);
		*/
	}

	// for check relative
	GetCursorPos(&m_ptFirstMousePos);
	m_ptCurMousePos = m_ptFirstMousePos;
	m_ptLastMousePos = m_ptCurMousePos;

	m_bRecording = TRUE;
	m_bRecordJustMove = ((CButton*)GetDlgItem(IDC_CHK_REC_MOUSEMOVE))->GetCheck();
	m_bRecordDelay = ((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->GetCheck();

	ToggleUIEnable(m_bRecording);
}

void CNaMacroRecorderDlg::RecordStop()
{
	if (!m_bRecording)
		return;

	TRACE(L"Stop Recording.\n");

	RAWINPUTDEVICE rawInputDev[2];
	ZeroMemory(rawInputDev, sizeof(RAWINPUTDEVICE) * 2);

	// key
	rawInputDev[0].usUsagePage = 0x01;
	rawInputDev[0].usUsage = 0x06;
	rawInputDev[0].dwFlags = RIDEV_REMOVE;
	rawInputDev[0].hwndTarget = NULL;

	// mouse
	rawInputDev[1].usUsagePage = 0x01;
	rawInputDev[1].usUsage = 0x02;
	rawInputDev[1].dwFlags = RIDEV_REMOVE; 
	rawInputDev[1].hwndTarget = NULL;

	if (RegisterRawInputDevices(rawInputDev, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		/*
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		MessageBox(str);
		*/
	}

	m_bRecording = FALSE;
	ToggleUIEnable(m_bRecording);

	// For Test
	POINT ptLastPos;
	ptLastPos.x = -1;
	ptLastPos.y = -1;

	DWORD dwLastTick = -1;

	CString strOutput;
	std::vector<ActionRecord>::iterator it = m_vecActionRecords.begin();
	for (; it != m_vecActionRecords.end(); ++it)
	{
		ActionRecord ar = *it;

		CString str;
		CString strJs;
		str.Format(L"TimeStamp: %ld, Action: %d, ", ar.dwTimeStamp, ar.enType);

		if (m_bRecordDelay &&
			dwLastTick != -1 && dwLastTick != ar.dwTimeStamp)
		{
			strJs.Format(L"sleep(%d);\n", ar.dwTimeStamp - dwLastTick);
			strOutput += strJs;
		}
		dwLastTick = ar.dwTimeStamp;

		if (ar.enType >= ACTION_MOUSEBEGIN && ar.enType <= ACTION_MOUSELAST)
		{
			str.Format(L"%sPos: %d, %d", str, ar.ptPos.x, ar.ptPos.y);

			BOOL bMoved = FALSE;
			if (ar.enType == ACTION_MOUSEMOVE || (!m_bRecordJustMove))
			{
				if (ar.ptPos.x != ptLastPos.x || ar.ptPos.y != ptLastPos.y)
					bMoved = TRUE;
			}

			if (bMoved)
			{
				strJs.Format(L"system.mouse.move(%d,%d);\n", ar.ptPos.x, ar.ptPos.y);
				strOutput += strJs;

				ptLastPos = ar.ptPos;
			}

			switch (ar.enType)
			{
			case ACTION_MOUSEMOVE:
				break;
			case ACTION_MOUSELBUTTONDOWN:
				strJs.Format(L"system.mouse.lbuttonDown();\n");
				strOutput += strJs;
				break;
			case ACTION_MOUSELBUTTONUP:
				strJs.Format(L"system.mouse.lbuttonUp();\n");
				strOutput += strJs;
				break;
			}
		}
		else if (ar.enType >= ACTION_KEYBEGIN && ar.enType <= ACTION_KEYLAST)
		{
			str.Format(L"%sKeyCode: %d (%c)", str, ar.nKeyCode, ar.nKeyCode);
			strJs.Format(L"system.keyboard.%s(%d);\n", 
				(ar.enType == ACTION_KEYDOWN) ? L"down" : L"up",
				ar.nKeyCode);
			strOutput += strJs;
		}
		str.Format(L"%s\n", str);
		TRACE(str);
	}

	m_vecActionRecords.clear();
	MessageBox(strOutput);

	// Copy to clipboard
	{
		strOutput.Replace(L"\n", L"\r\n");

		if (!OpenClipboard())
		{
			AfxMessageBox(_T("클립보드를 열 수 없습니다."));
			return;
		}
		EmptyClipboard();

		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, strOutput.GetLength() + 1);
		char *pchData = (char *)GlobalLock(hClipboardData);
		int nLen = WideCharToMultiByte(CP_ACP, 0, strOutput.GetBuffer(0), -1, NULL, 0, NULL, NULL);

		WideCharToMultiByte(CP_ACP, 0, strOutput.GetBuffer(0), -1, pchData, nLen, NULL, NULL);
		
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();
	}
}

void CNaMacroRecorderDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.

	RAWINPUT RawInput;
	unsigned int unSzRawInput = sizeof(RAWINPUT);
	GetRawInputData(hRawInput, RID_INPUT, &RawInput, &unSzRawInput, sizeof(RAWINPUTHEADER));

	BOOL bIgnore = FALSE;
	ActionRecord ar;

	switch (RawInput.header.dwType)
	{
	case RIM_TYPEKEYBOARD:
		{
			RAWKEYBOARD &rk = RawInput.data.keyboard;
			TRACE(L"OnRawInput.Keyboard) %d\n", rk.Flags);
			
			if (rk.Flags == RI_KEY_MAKE) // 0
			{
				ar.enType = ACTION_KEYDOWN;
				ar.nKeyCode = rk.VKey;
			}
			else if (rk.Flags & RI_KEY_BREAK)
			{
				ar.enType = ACTION_KEYUP;
				ar.nKeyCode = rk.VKey;
			}
		}
		break;
	case RIM_TYPEMOUSE:
		{
			RAWMOUSE &rm = RawInput.data.mouse;

			// RAWMOUSE Input does not check screen bound.
			// -> cannot check real position

			GetCursorPos(&m_ptCurMousePos);

			TRACE(L"OnRawInput.Mouse) flag: %d, btn-flag: %d, rel: (%d,%d), abs: (%d,%d)\n",
				rm.usFlags, rm.usButtonFlags,
				rm.lLastX, rm.lLastY,
				m_ptCurMousePos.x, m_ptCurMousePos.y
				);

			if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				ar.enType = ACTION_MOUSELBUTTONDOWN;
			}
			else if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				ar.enType = ACTION_MOUSELBUTTONUP;
			}
			else if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			{
				ar.enType = ACTION_MOUSERBUTTONDOWN;
			}
			else  if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			{
				ar.enType = ACTION_MOUSERBUTTONUP;
			}
			/*
			else if (...)
			{

			}
			*/
			else
			{
				ar.enType = ACTION_MOUSEMOVE;
				
				if (m_ptLastMousePos.x == m_ptCurMousePos.x &&
					m_ptLastMousePos.y == m_ptCurMousePos.y)
					bIgnore = TRUE;

				if (!m_bRecordJustMove)
					bIgnore = TRUE;

				m_ptLastMousePos = m_ptCurMousePos;
			}

			ar.ptPos = m_ptCurMousePos;
		}
		break;
	}

	if (!bIgnore)
	{
		DWORD dwTick = GetTickCount();

		ar.dwTimeStamp = dwTick;
		m_vecActionRecords.push_back(ar);
	}

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CNaMacroRecorderDlg::ToggleUIEnable(BOOL bRecording)
{
	GetDlgItem(IDC_BTN_REC)->EnableWindow(!bRecording);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(bRecording);

	GetDlgItem(IDC_CHK_REC_MOUSEMOVE)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_DELAY)->EnableWindow(!bRecording);
}
