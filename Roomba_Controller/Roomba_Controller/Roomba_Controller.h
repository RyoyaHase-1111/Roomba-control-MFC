
// Roomba_Controller.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CRoomba_ControllerApp:
// ���̃N���X�̎����ɂ��ẮARoomba_Controller.cpp ���Q�Ƃ��Ă��������B
//

class CRoomba_ControllerApp : public CWinApp
{
public:
	CRoomba_ControllerApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CRoomba_ControllerApp theApp;