//==============================================================================
//
//			CLaunch Plugin Framework
//
//			Copyright (C) 2005-2020 Pyonkichi
//
//==============================================================================
// Version 1.07														  2020/07/01
//==============================================================================

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"
#include "ClPlugin.h"

//==============================================================================
// ���� : �v���O�C�����������Ԃ�
//      :
// ���� : pszTitle  �v���O�C����������擾�o�b�t�@
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClGetPluginTitle(LPTSTR pszTitle)
{
	LoadString(hInst, IDS_TITLE, pszTitle, MAX_PLUGIN_TITLE);					// �����񃊃\�[�X�擾
}


//==============================================================================
// ���� : �v���O�C������������
//      :
// ���� : id      �v���O�C���o�^ID (-1:�ݒ�_�C�A���OBOX�p�̈ꎞ�o�^)
//		: pvFunc  CPI�֐��A�h���X�e�[�u���̃|�C���^
//      :
// �ߒl : TRUE:����I��  FALSE:�ُ�I��
//==============================================================================
BOOL ClPluginInit(int id, LPVOID *pvFunc)
{
	pCpiFunc = pvFunc;															// CPI�֐��A�h���X�e�[�u���̃|�C���^�ۑ�
	if (CpiGetVersion() < SUPPORT_VERSION)										// �{�̃o�[�W�������Â��H
	{
		return(FALSE);															// �������������~
	}
	// �ݒ��ǂݍ��ޕK�v������ꍇ�ɂ͂����ɋL�q

	if (id < 0)																	// CLaunch�I�v�V�����_�C�A���OBOX�p�̈ꎞ�o�^�H
	{
		return(TRUE);															// �����ŏI��
	}
	//--------------------------------------------------------------------------
	// ����ȍ~�͎��ۂɃv���O�C���o�^���ꂽ�ꍇ�̏����i�ꎞ�o�^�ł͂Ȃ��j
	//--------------------------------------------------------------------------
	iID = id;																	// �v���O�C���o�^ID�ۑ�
	// ���{�^���o�^�\�ȃv���O�C���̏ꍇ�͂����ŃA�C�e���o�^���s��
	TCHAR	szName[MAX_PLUGIN_TITLE];
	LoadString(hInst, IDS_ITEM_NAME, szName, BufLen(szName));					// �A�C�e����������擾
	CpiRegisterButtonItem(iID, szName);										// �{�^���A�C�e���ɓo�^
	return(TRUE);																// ����I��
}


//==============================================================================
// ���� : �v���O�C���I������
//      :
// ���� : id  �v���O�C���o�^ID (-1:�ݒ�_�C�A���OBOX�p�̈ꎞ�o�^����)
//      :
// �ߒl : TRUE:����I��  FALSE:�ُ�I��
//==============================================================================
BOOL ClPluginExit(int id)
{

	return(TRUE);																// ����I��
}


//==============================================================================
// ���� : �v���O�C���A�C�e���f�[�^����������
//		: �E�v���O�C���A�C�e�����{�^���ɓo�^���ꂽ��A�Ăяo����܂��B
//      :
// ���� : pBI  �������Ώۃ{�^�����̃|�C���^
//      :
// �ߒl : TRUE:����I��  FALSE:�ُ�I��
//==============================================================================
BOOL ClPluginInitItem(LPBTNINFO pBI)
{
	// �K�v�ɉ����Ĉȉ��̃f�[�^�ɏ����l���Z�b�g
	//	lstrcpy(pBI->szParam, ...
	//	lstrcpy(pBI->szDir, ...
	//	pBI->iSW = ...
	pBI->uFlags = 6;//�u�ʏ�v�D��x�A�R���\�[���E�C���h�E�͒ʏ�
	return(TRUE);																// ����I��
}


//==============================================================================
// ���� : �v���O�C���A�C�e�����s����
//      :
// ���� : pBI      �ΏۃA�C�e���{�^�����̃|�C���^
//		: uOption  �N���I�v�V�����t���O�i0x00000001:�{�^���������ŋN���j
//      :
// �ߒl : TRUE:����I��  FALSE:�ُ�I��
//==============================================================================
BOOL ClPluginExecute(LPBTNINFO pBI, UINT uOption)
{
	//prepare STARTUPINFO
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	UINT prio = 0x20;
	switch (pBI->uFlags >> 1) {
	case 0: prio = 0x100; break;
	case 1: prio = 0x80; break;
	case 2: prio = 0x8000; break;
	case 3: prio = 0x20; break;
	case 4: prio = 0x4000; break;
	case 5: prio = 0x40; break;
	}
	//set 0x08000000 if checked
	if (!CreateProcess(NULL, pBI->szParam, NULL, NULL, NULL, (pBI->uFlags & 0x1) << 27 | prio, NULL, pBI->szDir[0] ? pBI->szDir: NULL, &si, &pi))
		MessageBox(0, L"�N���Ɏ��s���܂����B�R�}���h�ƍ�ƃf�B���N�g�����m�F���Ă��������B", NULL, 0);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (uOption & EXE_PRESS_HOLD)												// �{�^���������ŋN�����ꂽ�H
	{
		// �{�^���������̏ꍇ�̏���
	}
	else																		// �������ł͂Ȃ�
	{
		// �ʏ�N�����ꂽ�ꍇ�̏���
	}
	return(TRUE);																// ����I��
}


//==============================================================================
// ���� : CLaunch���\�����ꂽ���̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnShowWindow(DWORD dwStatus)
{

}


//==============================================================================
// ���� : CLaunch����\���ɂȂ������̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnHideWindow(DWORD dwStatus)
{

}


//==============================================================================
// ���� : �A�C�e�����N�������O�̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �N���A�C�e�����܂ރy�[�W�̔ԍ�
//		: iButton   �N���A�C�e���̃{�^���ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnPreExecute(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// ���� : �A�C�e�����N�����ꂽ��̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �N���A�C�e�����܂ރy�[�W�̔ԍ�
//		: iButton   �N���A�C�e���̃{�^���ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnExecute(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// ���� : CLaunch�̃y�[�W���؂�ւ�������̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �V�����\�����ꂽ�y�[�W�̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnPageChange(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// ���� : �z�o�[��Ԃ̃^�u���؂�ւ�������̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �z�o�[��ԂɂȂ����y�[�W�̔ԍ� (-1:�z�o�[���OFF)
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnTabHover(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// ���� : �z�o�[��Ԃ̃{�^�����؂�ւ�������̏���
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iBtnPos   �z�o�[��ԂɂȂ����{�^���̈ʒu�ԍ� (-1:�z�o�[���OFF)
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnButtonHover(DWORD dwStatus, int iBtnPos)
{

}


//==============================================================================
// ���� : �y�[�W�ǉ�������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �ǉ����ꂽ�y�[�W�̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnAddPage(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// ���� : �y�[�W�폜������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �폜�����y�[�W�̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnDelPage(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// ���� : �{�^���ǉ�������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �{�^�����ǉ����ꂽ�y�[�W�̔ԍ�
//		: iButton   �ǉ����ꂽ�{�^���̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnAddButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// ���� : �{�^���폜������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �폜�����{�^�����܂ރy�[�W�̔ԍ�
//		: iButton   �폜�����{�^���̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnDelButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// ���� : �y�[�W�ړ�������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iSrc      �ړ����y�[�W�ʒu
//		: iDst      �ړ���y�[�W�ʒu
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnMovePage(DWORD dwStatus, int iSrc, int iDst)
{

}


//==============================================================================
// ���� : �{�^���ړ��������i�y�[�W�O�ւ̃{�^���ړ��̂݁j
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//		: iPage     �ړ����y�[�W�̔ԍ�
//		: iButton   �ړ����{�^���̔ԍ�
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnMoveButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// ���� : �E�B���h�E���b�N�ؑւ�������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnWindowLock(DWORD dwStatus)
{
	if (dwStatus & CS_PINNED)													// �E�B���h�E���b�N�H
	{
		// �E�B���h�E���b�N��ԂɂȂ������̏���
	}
	else																		// �E�B���h�E���b�N����
	{
		// �E�B���h�E���b�N���������ꂽ���̏���
	}
}


//==============================================================================
// ���� : �������[�h�ؑւ�������
//      :
// ���� : dwStatus  CLaunch�X�e�[�^�X�t���O
//      :
// �ߒl : �Ȃ�
//==============================================================================
void ClOnRestrictionMode(DWORD dwStatus)
{
	if (dwStatus & CS_LIMITMODE)												// �������[�h���H
	{
		// �������[�h�ɐ؂�ւ�������̏���
	}
	else																		// �������[�h����
	{
		// �������[�h���������ꂽ���̏���
	}
}


//==============================================================================
// ���� : �v���O�C���A�C�e���̃v���p�e�B�_�C�A���O�v���V�[�W��
//      :
// ���� : hDlg    �E�B���h�E�n���h��
//		: uMsg    ���b�Z�[�W
//		: wParam  wParam
//		: lParam  lParam
//      :
// �ߒl : ��������
//==============================================================================
LRESULT CALLBACK ClPluginItemPropDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LPBTNINFO pBI = NULL;

	switch (uMsg)
	{
		//----------------------------------------------------------------------
		// WM_INITDIALOG�F�_�C�A���O����������
		//----------------------------------------------------------------------
	case WM_INITDIALOG:
		pBI = (LPBTNINFO)lParam;    // �p�����[�^�̃{�^�����|�C���^��ۑ�

		if (pBI)
		{
			// szParam �̓��e���e�L�X�g�{�b�N�X�ɃZ�b�g
			SetDlgItemText(hDlg, IDC_EDIT_CMD, pBI->szParam);
			SetDlgItemText(hDlg, IDC_EDIT_WD, pBI->szDir);
			CheckDlgButton(hDlg, IDC_CHECK_CREATECONSOLE, pBI->uFlags & 0x1);

			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"���A���^�C��");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"��");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"�ʏ�ȏ�");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"�ʏ�");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"�ʏ�ȉ�");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"��");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_COMBO_PRIORITY), (pBI->uFlags & 0b1110) >> 1);
		}

		return TRUE;    // �R���g���[���Ƀt�H�[�J�X���Z�b�g���Ă��Ȃ�

		//----------------------------------------------------------------------
		// WM_COMMAND�F
		//----------------------------------------------------------------------
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:   // OK�{�^���������ꂽ
			if (pBI != NULL)
			{
				// �e�L�X�g�{�b�N�X�̓��e���擾���� szParam �Ɋi�[
				GetDlgItemText(hDlg, IDC_EDIT_CMD, pBI->szParam, ARRAYSIZE(pBI->szParam));
				GetDlgItemText(hDlg, IDC_EDIT_WD, pBI->szDir, ARRAYSIZE(pBI->szDir));
				int sel = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_COMBO_PRIORITY));
				UINT createConsole = IsDlgButtonChecked(hDlg, IDC_CHECK_CREATECONSOLE);
				pBI->uFlags = (sel << 1) | createConsole;
			}
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:  // �L�����Z���{�^���������ꂽ
			EndDialog(hDlg, FALSE);
			break;
		}
		return TRUE;    // ���b�Z�[�W�����ς�
	}
	return FALSE;   // ���b�Z�[�W������
}


//==============================================================================
// ���� : �v���O�C���ݒ�_�C�A���O�v���V�[�W��
//      :
// ���� : hDlg    �E�B���h�E�n���h��
//		: uMsg    ���b�Z�[�W
//		: wParam  wParam
//		: lParam  lParam
//      :
// �ߒl : ��������
//==============================================================================
LRESULT CALLBACK ClPluginSettingDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//--------------------------------------------------------------------------
	//	WM_INITDIALOG�F�_�C�A���O����������
	//--------------------------------------------------------------------------
	  case WM_INITDIALOG:
		return(TRUE);															// �R���g���[���Ƀt�H�[�J�X���Z�b�g���Ă��Ȃ�

	//--------------------------------------------------------------------------
	//	WM_COMMAND�F
	//--------------------------------------------------------------------------
	  case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		  case IDOK:															// OK�{�^���������ꂽ�H
			EndDialog(hDlg, TRUE);												// �_�C�A���OBOX�����
			break;

		  case IDCANCEL:														// �L�����Z���{�^���������ꂽ�H
			EndDialog(hDlg, FALSE);												// �_�C�A���OBOX�����
			break;
		}
		return(TRUE);															// ���b�Z�[�W�����ς�
	}
	return(FALSE);																// ���b�Z�[�W������
}


//==============================================================================
// ���� : �o�[�W�������_�C�A���O�v���V�[�W��
//      :
// ���� : hDlg    �E�B���h�E�n���h��
//		: uMsg    ���b�Z�[�W
//		: wParam  wParam
//		: lParam  lParam
//      :
// �ߒl : ��������
//==============================================================================
LRESULT CALLBACK ClAboutPluginDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR	szName[MAX_PLUGIN_TITLE+1];

	switch (uMsg)
	{
	//--------------------------------------------------------------------------
	//	WM_INITDIALOG�F�_�C�A���O����������
	//--------------------------------------------------------------------------
	  case WM_INITDIALOG:
		ClGetPluginTitle(szName);												// �v���O�C�����擾
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC1), szName);					// �X�^�e�B�b�N�R���g���[���P�ɃZ�b�g
		LoadString(hInst, IDS_COPYRIGHT, szName, BufLen(szName));				// �����񕶎���擾
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC2), szName);					// �X�^�e�B�b�N�R���g���[���Q�ɃZ�b�g
		return(TRUE);															// �R���g���[���Ƀt�H�[�J�X���Z�b�g���Ă��Ȃ�

	//--------------------------------------------------------------------------
	//	WM_COMMAND�F
	//--------------------------------------------------------------------------
	  case WM_COMMAND:
		if (LOWORD(wParam) == IDOK												// OK�{�^���������ꂽ�H
		||	LOWORD(wParam) == IDCANCEL)											// or ESC�L�[�������ꂽ�H
		{
			EndDialog(hDlg, FALSE);												// �_�C�A���OBOX�����
		}
		return(TRUE);															// ���b�Z�[�W�����ς�
	}
	return(FALSE);																// ���b�Z�[�W������
}


//==============================================================================
// ���� : �v���O�C���C�x���g�n���h�� ���C���֐�
//      :
// ���� : uEvent    �C�x���g�ԍ�
//		: lpParam1  �p�����[�^�P
//		: lpParam2  �p�����[�^�Q
//      :
// �ߒl : TRUE:����������  FALSE:���������s
//==============================================================================
BOOL ClPluginEventHandler(UINT uEvent, LONG_PTR lpParam1, LONG_PTR lpParam2)
{
	switch (uEvent)																// �C�x���g�ԍ��ɂ�鏈���I��
	{
	  //------------------------------------------------------------------------
	  // �v���O�C�����擾�v��
	  //------------------------------------------------------------------------
	  case PRQ_GET_TITLE:
		ClGetPluginTitle((LPTSTR)lpParam1);										// �v���O�C����������擾
		break;
	  //------------------------------------------------------------------------
	  // �v���O�C���������v��
	  //------------------------------------------------------------------------
	  case PRQ_INIT:
		return(ClPluginInit((int)lpParam1, (LPVOID*)lpParam2));					// �v���O�C������������
	  //------------------------------------------------------------------------
	  // �v���O�C���I���v��
	  //------------------------------------------------------------------------
	  case PRQ_EXIT:
		return(ClPluginExit((int)lpParam1));									// �v���O�C���I������
	  //------------------------------------------------------------------------
	  // �v���O�C��ID�ύX�v��
	  //------------------------------------------------------------------------
	  case PRQ_ID_CHANGE:
		iID = (int)lpParam1;													// �V�����v���O�C��ID��ۑ�
		break;
	  //------------------------------------------------------------------------
	  // �v���O�C���A�C�e���f�[�^�������v��
	  //------------------------------------------------------------------------
	  case PRQ_INIT_ITEM:
		return(ClPluginInitItem((LPBTNINFO)lpParam1));							// �v���O�C���A�C�e���f�[�^����������
	  //------------------------------------------------------------------------
	  // �v���O�C���A�C�e�����s�v��
	  //------------------------------------------------------------------------
	  case PRQ_EXECUTE:
		return(ClPluginExecute((LPBTNINFO)lpParam1, (UINT)lpParam2));			// �v���O�C���A�C�e�����s����
	  //------------------------------------------------------------------------
	  // �v���O�C���A�C�e���̃v���p�e�B�_�C�A���O�\���v��
	  //------------------------------------------------------------------------
	  case PRQ_ITEM_PROP:
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_PROPERTY), (HWND)lpParam1,
							(DLGPROC)ClPluginItemPropDlg, (LPARAM)lpParam2);	// �v���O�C���ݒ�_�C�A���O�\��
		break;
	  //------------------------------------------------------------------------
	  // �v���O�C���ݒ�_�C�A���O�\���v��
	  //------------------------------------------------------------------------
	  case PRQ_SETTING:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTING),
								(HWND)lpParam1, (DLGPROC)ClPluginSettingDlg);	// �v���O�C���ݒ�_�C�A���O�\��
		break;
	  //------------------------------------------------------------------------
	  // �v���O�C���o�[�W�������_�C�A���O�\���v��
	  //------------------------------------------------------------------------
	  case PRQ_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT),
								(HWND)lpParam1, (DLGPROC)ClAboutPluginDlg);		// �o�[�W�������_�C�A���O�\��
		break;
	  //------------------------------------------------------------------------
	  // CLaunch�\��
	  //------------------------------------------------------------------------
	  case PEV_SHOW:
		ClOnShowWindow((DWORD)lpParam1);										// CLaunch�\���C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // CLaunch��\��
	  //------------------------------------------------------------------------
	  case PEV_HIDE:
		ClOnHideWindow((DWORD)lpParam1);										// CLaunch��\���C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �A�C�e���N���O
	  //------------------------------------------------------------------------
	  case PEV_PRE_EXECUTE:
		ClOnPreExecute((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));	// �A�C�e���N���O�C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �A�C�e���N����
	  //------------------------------------------------------------------------
	  case PEV_EXECUTE:
		ClOnExecute((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// �A�C�e���N����C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �y�[�W�ؑւ�
	  //------------------------------------------------------------------------
	  case PEV_PAGE_CHANGE:
		ClOnPageChange((DWORD)lpParam1, (int)lpParam2);							// �y�[�W�ؑւ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �^�u�̃z�o�[��ԕω�
	  //------------------------------------------------------------------------
	  case PEV_TAB_HOVER:
		ClOnTabHover((DWORD)lpParam1, (int)lpParam2);							// �^�u�̃z�o�[��ԕω��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �{�^���̃z�o�[��ԕω�
	  //------------------------------------------------------------------------
	  case PEV_BUTTON_HOVER:
		ClOnButtonHover((DWORD)lpParam1, (int)lpParam2);						// �{�^���̃z�o�[��ԕω��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �y�[�W�ǉ�
	  //------------------------------------------------------------------------
	  case PEV_ADD_PAGE:
		ClOnAddPage((DWORD)lpParam1, (int)lpParam2);							// �y�[�W�ǉ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �y�[�W�폜
	  //------------------------------------------------------------------------
	  case PEV_DEL_PAGE:
		ClOnDelPage((DWORD)lpParam1, (int)lpParam2);							// �y�[�W�폜�C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �{�^���ǉ�
	  //------------------------------------------------------------------------
	  case PEV_ADD_BUTTON:
		ClOnAddButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// �{�^���ǉ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �{�^���폜
	  //------------------------------------------------------------------------
	  case PEV_DEL_BUTTON:
		ClOnDelButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// �{�^���폜�C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �y�[�W�ړ�
	  //------------------------------------------------------------------------
	  case PEV_MOVE_PAGE:
		ClOnMovePage((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// �y�[�W�ړ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �{�^���ړ��i�y�[�W�O�ւ̃{�^���ړ��̂݁j
	  //------------------------------------------------------------------------
	  case PEV_MOVE_BUTTON:
		ClOnMoveButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));	// �{�^���ړ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �E�B���h�E���b�N�ؑւ�
	  //------------------------------------------------------------------------
	  case PEV_WIN_LOCK:
		ClOnWindowLock((DWORD)lpParam1);										// �E�B���h�E���b�N�ؑւ��C�x���g����
		break;
	  //------------------------------------------------------------------------
	  // �������[�h�ؑւ�
	  //------------------------------------------------------------------------
	  case PEV_RESTRICT_MODE:
		ClOnRestrictionMode((DWORD)lpParam1);									// �������[�h�ؑւ��C�x���g����
		break;
	}
	return(TRUE);																// ����I��
}


//==============================================================================
// ���� : DLL�ʒm�G���g���|�C���g
//		:
// ���� : hinstDLL     DLL ���W���[���̃n���h��
//		: dwReason     �֐����Ăяo�����R
//		: lpvReserved  �\��ς�
//      :
// �ߒl : TRUE:����  FALSE:���s
//==============================================================================
BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hInstDLL;														// DLL�̃C���X�^���X�n���h���ۑ�
	}
	return(TRUE);
}


