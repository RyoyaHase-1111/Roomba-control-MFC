
// Roomba_Controller.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CRoomba_ControllerApp:
// このクラスの実装については、Roomba_Controller.cpp を参照してください。
//

class CRoomba_ControllerApp : public CWinApp
{
public:
	CRoomba_ControllerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CRoomba_ControllerApp theApp;