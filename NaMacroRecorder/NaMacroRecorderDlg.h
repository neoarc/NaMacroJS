#pragma once

#include <vector>
#include "afxcmn.h"

#include "ActionRecord.h"

enum RecordMouseMoveOptions 
{
	RECORD_MOUSEMOVE_NONE,
	RECORD_MOUSEMOVE_CLICKED, 
	RECORD_MOUSEMOVE_ALL,	
};

struct FileInfo
{
	CString strFullPath;
	CString strPath;
	CString strFileName;
};

class CNaMacroRecorderDlg : public CDialogEx
{
public:
	CNaMacroRecorderDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAMACRORECORDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

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
	afx_msg void OnBnClickedChkRecMousemove();
	afx_msg void OnBnClickedChkRecClickmove();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL IsMouseClicked();
	void StartRecord();
	void StopRecord();

	bool IsAddWindowInfo();
	bool IsUseRelativeCoord();

	void SaveRecordToNaMacroScript(IN CString filename);

	CString GetKeyName(unsigned int nKey);

	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);

	// recording state
	BOOL m_bRecording;

	// recording options
	RecordMouseMoveOptions m_enRecordMove;
	BOOL m_bRecordDelay;

	// recording data
	HWND m_hActiveWnd;

	POINT m_ptFirstMousePos;
	POINT m_ptCurMousePos;
	POINT m_ptLastMousePos;
	std::vector<ActionRecord*> m_vecActionRecords;
	
	void ToggleUIEnable(BOOL bRecording);	

	// Control variable
	CListCtrl m_listFiles;

	// Config control
	void LoadFiles();
	void LoadConfig(CString &strId, CString &strVal);
	void AddFile(CString &strFullPath);
	void SaveFiles();

private:
	bool CopyToClipboard(CString& s);
	void StartCaptureInputDevice();
	void StopCaptureInputDevice();
};
