
// NaMacroRecorderDlg.h : 헤더 파일
//

#pragma once

#include <vector>

enum ActionTypes {
	ACTION_MOUSEBEGIN,
	ACTION_MOUSEMOVE,
	ACTION_MOUSELBUTTONDOWN,
	ACTION_MOUSELBUTTONUP,
	ACTION_MOUSERBUTTONDOWN,
	ACTION_MOUSERBUTTONUP,
	ACTION_MOUSELAST,

	ACTION_KEYBEGIN,
	ACTION_KEYDOWN,
	ACTION_KEYUP,
	ACTION_KEYLAST,
};

enum RecordMouseMoveOptions {
	RECORD_MOUSEMOVE_NONE,
	RECORD_MOUSEMOVE_CLICKED, 
	RECORD_MOUSEMOVE_ALL,	
};

union ActionRecord {
	// mouse action
	struct {
		ActionTypes enType;
		POINT ptPos;
		DWORD dwTimeStamp;
	};
	// key action
	struct {
		ActionTypes enType;
		int nKeyCode;
		DWORD dwTimeStamp;
	};
};

// CNaMacroRecorderDlg 대화 상자
class CNaMacroRecorderDlg : public CDialogEx
{
// 생성입니다.
public:
	CNaMacroRecorderDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAMACRORECORDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedBtnRec();
	afx_msg void OnBnClickedBtnStop();

	BOOL IsMouseClicked();
	void RecordStart();
	void RecordStop();
	void RecordToNaMacroScript(CString& strOutput);
	CString GetKeyName(unsigned int nKey);

	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);

	// recording state
	BOOL m_bRecording;

	// recording options
	RecordMouseMoveOptions m_enRecordMove;
	BOOL m_bRecordDelay;

	// recording data
	POINT m_ptFirstMousePos;
	POINT m_ptCurMousePos;
	POINT m_ptLastMousePos;
	std::vector<ActionRecord> m_vecActionRecords;
	
	void ToggleUIEnable(BOOL bRecording);
	afx_msg void OnBnClickedChkRecMousemove();
	afx_msg void OnBnClickedChkRecClickmove();
};
