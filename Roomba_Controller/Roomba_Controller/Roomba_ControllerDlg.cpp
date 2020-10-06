
// Roomba_ControllerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Roomba_Controller.h"
#include "Roomba_ControllerDlg.h"
#include "afxdialogex.h"

#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool con_Events = false, seri_Conn = false;
HANDLE  hComPort;
char sendData[10] = { 0 };
int sendDataSize = 0;
int speedVal;

void Int_to_Hex_for_Roomba(int n, unsigned char *a, unsigned char *b) {
	*a = n; //low
	n = (n >> 8);
	*b = n; //high
}

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CRoomba_ControllerDlg ダイアログ



CRoomba_ControllerDlg::CRoomba_ControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROOMBA_CONTROLLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRoomba_ControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
}

BEGIN_MESSAGE_MAP(CRoomba_ControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON4, &CRoomba_ControllerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CRoomba_ControllerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRoomba_ControllerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRoomba_ControllerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CRoomba_ControllerDlg::OnBnClickedButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CRoomba_ControllerDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON6, &CRoomba_ControllerDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CRoomba_ControllerDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CRoomba_ControllerDlg::OnBnClickedButton9)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CRoomba_ControllerDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CRoomba_ControllerDlg メッセージ ハンドラー

BOOL CRoomba_ControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	DCB dcb;
	COMMTIMEOUTS cto;

	hComPort = CreateFile(L"COM5",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hComPort == INVALID_HANDLE_VALUE) {
		TRACE("\n\nOpen Error!\n\n");
	}
	else {
		seri_Conn = true;

		GetCommState(hComPort, &dcb); // シリアルポートの状態を取得
		dcb.BaudRate = 115200;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.fParity = FALSE;
		dcb.StopBits = ONESTOPBIT;
		SetCommState(hComPort, &dcb);
	}
	TCHAR msg[8];

	m_slider.SetRange(0, 100);
	m_slider.SetPos(50);
	speedVal = m_slider.GetPos();
	wsprintf(msg, TEXT("%d"), speedVal);
	m_static2.SetWindowTextW(msg);

	SetTimer(1, 100, NULL);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CRoomba_ControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CRoomba_ControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CRoomba_ControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRoomba_ControllerDlg::OnBnClickedButton1()
{
	unsigned char hi[2], lo[2]; // right and left motor
	int i;
	memset(sendData, 0, sizeof(sendData));
	
	for (i = 0; i < 2; i++) {
		Int_to_Hex_for_Roomba(speedVal * 5, &lo[i], &hi[i]);
	}
	sprintf_s(sendData, "%c%c%c%c%c", 0x91, hi[0], lo[0], hi[1], lo[1]);
	sendDataSize = 5;
	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton2()
{
	unsigned char hi[2], lo[2]; // right and left motor
	int i;
	memset(sendData, 0, sizeof(sendData));

	for (i = 0; i < 2; i++) {
		if (i == 1) Int_to_Hex_for_Roomba(speedVal * (-5), &lo[i], &hi[i]);
		else Int_to_Hex_for_Roomba(speedVal * 5, &lo[i], &hi[i]);
	}
	sprintf_s(sendData, "%c%c%c%c%c", 0x91, hi[0], lo[0], hi[1], lo[1]);
	sendDataSize = 5;
	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton3()
{
	unsigned char hi[2], lo[2]; // right and left motor
	int i;
	memset(sendData, 0, sizeof(sendData));

	for (i = 0; i < 2; i++) {
		if(i == 1) Int_to_Hex_for_Roomba(speedVal * 5, &lo[i], &hi[i]);
		else Int_to_Hex_for_Roomba(speedVal * (-5), &lo[i], &hi[i]);
	}
	sprintf_s(sendData, "%c%c%c%c%c", 0x91, hi[0], lo[0], hi[1], lo[1]);
	sendDataSize = 5;
	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton4()
{
	unsigned char hi[2], lo[2]; // 0: right and 1: left motor
	int i;
	memset(sendData, 0, sizeof(sendData));

	for (i = 0; i < 2; i++) {
		Int_to_Hex_for_Roomba(speedVal * (-5), &lo[i], &hi[i]);
	}
	sprintf_s(sendData, "%c%c%c%c%c", 0x91, hi[0], lo[0], hi[1], lo[1]);
	sendDataSize = 5;
	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton5()
{
	unsigned char hi[2], lo[2]; // 0: right and 1: left motor
	int i;
	memset(sendData, 0, sizeof(sendData));

	for (i = 0; i < 2; i++) {
		Int_to_Hex_for_Roomba(0, &lo[i], &hi[i]);
	}
	sprintf_s(sendData, "%c%c%c%c%c", 0x91, hi[0], lo[0], hi[1], lo[1]);
	sendDataSize = 5;
	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton6()
{
	memset(sendData, 0, sizeof(sendData));

	sprintf_s(sendData, "%c", 0x80);
	sendDataSize = 1;

	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton7()
{
	memset(sendData, 0, sizeof(sendData));

	sprintf_s(sendData, "%c", 0x83);
	sendDataSize = 1;

	con_Events = true;
}


void CRoomba_ControllerDlg::OnBnClickedButton9()
{
	memset(sendData, 0, sizeof(sendData));

	sprintf_s(sendData, "%c", 0x07);
	sendDataSize = 1;

	con_Events = true;
}

void CRoomba_ControllerDlg::OnBnClickedButton8()
{
	memset(sendData, 0, sizeof(sendData));

	sprintf_s(sendData, "%c", 0xAD);
	sendDataSize = 1;

	con_Events = true;
}


void CRoomba_ControllerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		if (con_Events && seri_Conn) {
			DWORD wn = 0;
			WriteFile(hComPort, sendData, sendDataSize, &wn, NULL);
			con_Events = false;
		}
		TCHAR msg[8];
		if (speedVal != m_slider.GetPos()) {
			speedVal = m_slider.GetPos();
			con_Events = true;
			wsprintf(msg, TEXT("%d"), speedVal);
			m_static2.SetWindowTextW(msg);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}




void CRoomba_ControllerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}
