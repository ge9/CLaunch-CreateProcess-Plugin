//==============================================================================
//
//			CLaunch Plugin Header File
//
//			Copyright (C) 2005-2020 Pyonkichi
//
//==============================================================================
// Version 1.07														  2020/07/01
//==============================================================================
#include <shtypes.h>

//------------------------------------------------------------------------------
// CLaunch�{�̂̑Ή��o�[�W����
//	�v���O�C�����Ή��\��CLaunch�̖{�̃o�[�W������ݒ肵�Ă�������
#define	SUPPORT_VERSION		0x03000000
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �����l
//------------------------------------------------------------------------------
#define MAX_PLUGIN_TITLE	MAX_PATH											// �v���O�C�����ő咷
#define	MAX_PLUGINNAME		30													// �v���O�C���A�C�e����
#define MAX_PAGENAME		50													// �y�[�W�̖��ő咷
#define	MAX_BTNNAME			50													// �{�^��������
#define	MAX_CAPTION			100													// �L���v�V����������

//------------------------------------------------------------------------------
// �E�B���h�E�\���ʒu		(CpiShowWindow()�̑�P�p�����[�^)
//------------------------------------------------------------------------------
#define	WPOS_NOMOVE			0x00000000											// ���݈ʒu
#define	WPOS_CENTER			0x00000001											// ��ʒ���
#define	WPOS_MOUSE_CENTER	0x00000002											// �J�[�\���̎���
#define	WPOS_SPECIFY		0x00000003											// �w����W
#define	WPOS_MOUSE_LEFTOP	0x00000004											// �J�[�\���̉E��
#define	WPOS_AUTO_HIDE		0x00000100											// ������\���w��t���O
#define	WPOS_FORCE_SHOW		0x00000200											// �����\���w��t���O
#define	GETWINPOS(pos)		(pos & 0x000000ff)									// �\���ʒu�ԍ����o�}�N��

//------------------------------------------------------------------------------
// �����\���y�[�W			(CpiShowWindow()�̑�S�p�����[�^)
//------------------------------------------------------------------------------
#define	PN_MEMORY			0xffff												// �O��̃y�[�W�\��

//------------------------------------------------------------------------------
// �X�e�[�^�X�t���O			(CpiCheckStatus()�̃p�����[�^)
//------------------------------------------------------------------------------
#define	CS_DISPMODE			0x00000001											// �\�����[�h
#define	CS_PINNED			0x00000002											// �E�B���h�E���b�N��
#define	CS_LIMITMODE		0x00000004											// �������[�h��
#define	CS_SEARCHWND		0x00000008											// �����E�B���h�E

//------------------------------------------------------------------------------
// ����ݒ�I�v�V�����t���O	(CpiCheckSetting()�̃p�����[�^)
//------------------------------------------------------------------------------
#define	OPT_CLOSE_SRCHWND	0x00000001											// �A�C�e���N����Ɍ����E�B���h�E�����
#define	OPT_CANCEL_HOLDING	0x00000002											// �J�[�\�����ړ������璷�������L�����Z������
#define	OPT_CNV_RELATIVE	0x00000004											// �A�C�e���𑊑΃p�X�œo�^����
#define	OPT_UPDATE_TRASH	0x00000008											// ���ݔ��̃A�C�R�������X�V����
#define	OPT_AUTO_SPC_DEL	0x00000010											// �A�C�e���폜��ɋ󔒂������ŋl�߂�
#define	OPT_DND_HIDE		0x00000020											// �h���b�O���h���b�v�N����ɃE�B���h�E���\����
#define	OPT_USE_FILER		0x00000040											// �t�@�C���[���g�p����
#define	OPT_AUTO_BACKUP		0x00000080											// ����I�ɐݒ���o�b�N�A�b�v����
#define	OPT_AUTO_PAGESW		0x00000100											// �^�u�̃}�E�X�I�[�o�[�Ńy�[�W��ؑւ���
#define	OPT_CIRCLE_AUTOHIDE	0x00000200											// �J�[�\�����E�B���h�E�O�֏o�����\����(�T�[�N��)
#define	OPT_AUTO_MODECNG	0x00000400											// �A�C�e���N����ɕ\�����[�h1�֐ؑւ���
#define	OPT_DELAY_SINGLE	0x00000800											// �V���O���N���b�N�x�����s
#define	OPT_DELAY_HOTKEY	0x00001000											// �A���L�[���͂̃^�C���A�E�g��҂�
#define	OPT_PAGE_LOOP		0x00002000											// �y�[�W�ؑւ����[�v
#define	OPT_PROP_AFT_REG	0x00004000											// �A�C�e���o�^��v���p�e�B�\��
#define	OPT_EDGE_AUTOHIDE	0x00008000											// �J�[�\�����E�B���h�E�O�֏o�����\����(�G�b�W)
#define	OPT_HOTKEY_AUTOHIDE	0x00010000											// �J�[�\�����E�B���h�E�O�֏o�����\����(�z�b�g�L�[)
#define	OPT_SEND_MDBLCLK	0x00020000											// �z�C�[���_�u���N���b�N���̃I�[�g�X�N���[���h�~
#define	OPT_MENU_AUTOSCRL	0x00040000											// �X�N���[���{�^���̃}�E�X�I�[�o�[�Ŏ����X�N���[��(�T�u���j���[)
#define	OPT_CLICK_AUTOHIDE	0x00080000											// �J�[�\�����E�B���h�E�O�֏o�����\����(�N���b�N)
#define	OPT_FREE_MEMORY		0x00100000											// �s�v���������
#define	OPT_CLICK_HIPRI		0x00200000											// �N���b�N���菈���̗D��x���グ��
#define	OPT_SAVE_WINLOCK	0x00400000											// �E�B���h�E���b�N��Ԃ��L������
#define	OPT_SAVE_RESTRICT	0x00800000											// �������[�h��Ԃ��L������
#define	OPT_EDGE_PRESS		0x01000000											// �}�E�X�{�^���������̂݃G�b�W�\��
#define	OPT_MENU_KEYCURS	0x02000000											// �J�[�\���ʒu�̃��j���[���L�[����̑Ώۂɂ���(�T�u���j���[)
#define	OPT_MENU_DELAYICON	0x04000000											// ���j���[�\����ɃA�C�R�����擾����(�T�u���j���[�̃t�H���_�[�W�J��)
#define	OPT_CIRCLE_HIPRI	0x08000000											// �T�[�N�����菈���̗D��x���グ��
#define	OPT_ASSIGN_SKEY		0x10000000											// �A�C�e���o�^���ɃV���[�g�J�b�g�L�[�������Ŋ��蓖�Ă�
#define	OPT_INHSCR_KEYSEL	0x20000000											// �{�^���I�𑀍�ŃX�N���[�����Ȃ�
#define	OPT_UNUSED_BIT		0xc0000000											// �I�v�V�����ݒ�t���O�̖��g�p�r�b�g

//------------------------------------------------------------------------------
// �\���ݒ�I�v�V�����t���O	(CpiCheckDispSetting()�̃p�����[�^)
//------------------------------------------------------------------------------
#define	DSP_TOPMOST			0x00000001											// �őO�ʂɕ\������
#define	DSP_ALPHA_INACTIVE	0x00000002											// ��A�N�e�B�u���̂ݓ�����
#define	DSP_CAP_BTNNAME		0x00000004											// �L���v�V�����Ƀ{�^������\��
#define	DSP_CAP_BTNTIP		0x00000008											// �L���v�V�����Ƀc�[���`�b�v�������\��
#define	DSP_SHOW_TIPS		0x00000010											// �c�[���`�b�v��\������
#define	DSP_TIPS_TAB		0x00000020											// �^�u�̃c�[���`�b�v
#define	DSP_TIPS_BALLOON	0x00000040											// �o���[���^�c�[���`�b�v
#define	DSP_TRAY_ICON		0x00000080											// �g���C�A�C�R����\������
#define	DSP_CHANGE_TICON	0x00000100											// �g���C�A�C�R����ύX����
#define	DSP_MENU_LARGEICON	0x00000200											// �傫���A�C�R�����g�p����(�T�u���j���[)
#define	DSP_SHOW_MENUTIPS	0x00000400											// �c�[���`�b�v��\������(�T�u���j���[)
#define	DSP_WHILE_PINNED	0x00000800											// �E�B���h�E���b�N���̂�

//------------------------------------------------------------------------------
// �A�C�e���N�����ǉ��I�v�V�����t���O
//------------------------------------------------------------------------------
#define	EXE_PRESS_HOLD		0x00000001											// �{�^���������N��

//------------------------------------------------------------------------------
//	�\���̐錾
//------------------------------------------------------------------------------
//������������������������������������������������������������������������������
// �y�[�W���\����
//������������������������������������������������������������������������������
typedef struct {
	int			iNumBtn;														// �y�[�W���{�^����
	TCHAR		szPage[MAX_PAGENAME+1];											// �y�[�W��
} PAGEINFO, *LPPAGEINFO;

//������������������������������������������������������������������������������
// �{�^�����\����
// szParam, szDir, iSW, uFlags�ɂ̓v���O�C���A�C�e���̌ŗL�ݒ��ۑ��\
//������������������������������������������������������������������������������
typedef struct {
	int				iPage;														// �����y�[�W�̃C���f�b�N�X
	int				iNumber;													// �{�^���C���f�b�N�X
	int				iPos;														// �{�^���\���ʒu
	UINT			uType;														// �A�C�e���^�C�v
	TCHAR			szName[MAX_BTNNAME+1];										// �{�^����
	TCHAR			szTip[MAX_PATH];											// �c�[���`�b�v
	TCHAR			szFile[MAX_PATH];											// �t�@�C����
	TCHAR			szParam[MAX_PATH];											// �p�����[�^
	TCHAR			szDir[MAX_PATH];											// ���[�N�f�B���N�g��
	int				iSW;														// �E�B���h�E�����\�����
	WORD			wKeyboard;													// �V���[�g�J�b�g�L�[
	UINT			uFlags;														// �N�����I�v�V�����t���O
} BTNINFO, *LPBTNINFO;

//------------------------------------------------------------------------------
// �ėp�}�N��
//------------------------------------------------------------------------------
#define	BufLen(sz)		(sizeof(sz)/sizeof(TCHAR))								// ������o�b�t�@�̃T�C�Y(������)�擾

//------------------------------------------------------------------------------
// �C�x���g�ԍ�
//------------------------------------------------------------------------------
// �����v��
#define PRQ_GET_TITLE		0x00000001											// �v���O�C�����擾�v��
#define PRQ_INIT			0x00000002											// �v���O�C���������v��
#define PRQ_EXIT			0x00000003											// �v���O�C���I���v��
#define PRQ_ID_CHANGE		0x00000004											// �v���O�C��ID�ύX�v��
#define PRQ_INIT_ITEM		0x00000005											// �A�C�e���f�[�^�������v��
#define PRQ_EXECUTE			0x00000006											// �A�C�e�����s�v��
#define PRQ_ABOUT			0x00000007											// �v���O�C���o�[�W�������_�C�A���O�\���v��
#define PRQ_SETTING			0x00000008											// �v���O�C���ݒ�_�C�A���O�\���v��
#define PRQ_ITEM_PROP		0x00000009											// �v���O�C���A�C�e���̃v���p�e�B�_�C�A���O�\���v��
// �C�x���g
#define PEV_SHOW			0x00010001											// CLaunch�\��
#define PEV_HIDE			0x00010002											// CLaunch��\��
#define PEV_PRE_EXECUTE		0x00010003											// �A�C�e���N���O
#define PEV_EXECUTE			0x00010004											// �A�C�e���N����
#define PEV_PAGE_CHANGE		0x00010005											// �y�[�W�ؑւ�
#define PEV_TAB_HOVER		0x00010006											// �^�u�̃z�o�[��ԕω�
#define PEV_BUTTON_HOVER	0x00010007											// �{�^���̃z�o�[��ԕω�
#define PEV_ADD_PAGE		0x00010008											// �y�[�W�ǉ�
#define PEV_DEL_PAGE		0x00010009											// �y�[�W�폜
#define PEV_ADD_BUTTON		0x0001000a											// �{�^���ǉ�
#define PEV_DEL_BUTTON		0x0001000b											// �{�^���폜
#define PEV_MOVE_PAGE		0x0001000c											// �y�[�W�ړ�
#define PEV_MOVE_BUTTON		0x0001000d											// �{�^���ړ��i�y�[�W�O�ւ̈ړ��j
#define PEV_WIN_LOCK		0x0001000e											// �E�B���h�E���b�N�ؑւ�
#define PEV_RESTRICT_MODE	0x0001000f											// �������[�h�ؑւ�

//------------------------------------------------------------------------------
// CPI�֐���`
//------------------------------------------------------------------------------
// �֐��^�C�v�錾
typedef DWORD (*LPGET_VERSION)(void);											// CLaunch�̃o�[�W�������擾����
typedef void (*LPSHOW_WINDOW)(UINT uPos, int x, int y, int iPage);				// CLaunch��\������
typedef void (*LPHIDE_WINDOW)(void);											// CLaunch���\���ɂ���
typedef void (*LPGET_HANDLE)(HINSTANCE *phInst, HWND *phWnd);					// CLaunch�̃C���X�^���X�n���h���A�E�B���h�E�n���h�����擾����
typedef int (*LPGET_PAGENUM)(void);												// �y�[�W�����擾����
typedef int (*LPGET_CURPAGE)(void);												// ���݂̃y�[�W�ԍ����擾����
typedef LPPAGEINFO (*LPGET_PAGEINFO)(int iPage);								// �y�[�W�����擾����
typedef LPBTNINFO (*LPGET_BUTTONINFO)(int iPage, int iBtn);						// �w��y�[�W�A�w��{�^���̃{�^�������擾����
typedef LPBTNINFO (*LPGET_DISPBTNINFO)(int iPos);								// �\���y�[�W���A�w��ʒu�̃{�^�������擾����
typedef void (*LPSET_CAPTION)(int id, LPCTSTR pszCaption, BOOL bSave);			// �L���v�V������ύX����
typedef void (*LPREG_BUTTONITEM)(int id, LPCTSTR pszRegName);					// �v���O�C�����{�^���A�C�e���Ƃ��ēo�^����
typedef void (*LPCHANGE_PAGE)(int iPage);										// �w��y�[�W�ɐؑւ���
typedef void (*LPSELECT_BUTTON)(int iBtnPos);									// �w��ʒu�̃{�^����I������
typedef void (*LPEXECUTE_ITEM)(int iPage, int iButton);							// �w��A�C�e�����N������
typedef BOOL (*LPCHK_STATUS)(UINT uBit);										// CLaunch�̃X�e�[�^���擾����
typedef BOOL (*LPCHK_SETTING)(UINT uBit);										// CLaunch�̓���I�v�V�����ݒ���擾����
typedef BOOL (*LPCHK_DISPSETTING)(UINT uBit);									// CLaunch�̕\���֘A�I�v�V�����ݒ���擾����

// �֐���`
#define CpiGetVersion			((LPGET_VERSION)pCpiFunc[0])					// CLaunch�̃o�[�W�������擾����
#define CpiShowWindow			((LPSHOW_WINDOW)pCpiFunc[1])					// CLaunch��\������
#define CpiHideWindow			((LPHIDE_WINDOW)pCpiFunc[2])					// CLaunch���\���ɂ���
#define CpiGetHandle			((LPGET_HANDLE)pCpiFunc[3])						// CLaunch�̃C���X�^���X�n���h���A�E�B���h�E�n���h�����擾����
#define CpiGetPageNum			((LPGET_PAGENUM)pCpiFunc[4])					// �y�[�W�����擾����
#define CpiGetCurrentPage		((LPGET_CURPAGE)pCpiFunc[5])					// ���݂̃y�[�W�ԍ����擾����
#define CpiGetPageinfo			((LPGET_PAGEINFO)pCpiFunc[6])					// �y�[�W�����擾����
#define CpiGetButtonInfo		((LPGET_BUTTONINFO)pCpiFunc[7])					// �w��y�[�W�A�w��{�^���̃{�^�������擾����
#define CpiGetDispBtnInfo		((LPGET_DISPBTNINFO)pCpiFunc[8])				// �\���y�[�W���A�w��ʒu�̃{�^�������擾����
#define CpiSetCaption			((LPSET_CAPTION)pCpiFunc[9])					// �L���v�V������ύX����
#define CpiRegisterButtonItem	((LPREG_BUTTONITEM)pCpiFunc[10])				// �v���O�C�����{�^���A�C�e���Ƃ��ēo�^����
#define CpiChangePage			((LPCHANGE_PAGE)pCpiFunc[11])					// �w��y�[�W�ɐؑւ���
#define CpiSelectButton			((LPSELECT_BUTTON)pCpiFunc[12])					// �w��ʒu�̃{�^����I������
#define CpiExecuteItem			((LPEXECUTE_ITEM)pCpiFunc[13])					// �w��A�C�e�����N������
#define CpiCheckStatus			((LPCHK_STATUS)pCpiFunc[14])					// CLaunch�̃X�e�[�^���擾����
#define CpiCheckSetting			((LPCHK_SETTING)pCpiFunc[15])					// CLaunch�̓���I�v�V�����ݒ���擾����
#define CpiCheckDispSetting		((LPCHK_DISPSETTING)pCpiFunc[16])				// CLaunch�̕\���֘A�I�v�V�����ݒ���擾����

//------------------------------------------------------------------------------
// �v���O�C���J���L�b�g���֐��v���g�^�C�v�錾
//------------------------------------------------------------------------------
void ClGetPluginTitle(LPTSTR pszTitle);
BOOL ClPluginInit(int id, LPVOID *pvFunc);
BOOL ClPluginExit(int id);
BOOL ClPluginInitItem(LPBTNINFO pBI);
BOOL ClPluginExecute(LPBTNINFO pBI, UINT uOption);
void ClOnShowWindow(DWORD dwStatus);
void ClOnHideWindow(DWORD dwStatus);
void ClOnPreExecute(DWORD dwStatus, int iPage, int iButton);
void ClOnExecute(DWORD dwStatus, int iPage, int iButton);
void ClOnPageChange(DWORD dwStatus, int iPage);
void ClDrawTabHover(DWORD dwStatus, int iPage);
void ClOnButtonHover(DWORD dwStatus, int iBtnPos);
void ClOnAddPage(DWORD dwStatus, int iPage);
void ClOnDelPage(DWORD dwStatus, int iPage);
void ClOnAddButton(DWORD dwStatus, int iPage, int iButton);
void ClOnDelButton(DWORD dwStatus, int iPage, int iButton);
void ClOnMovePage(DWORD dwStatus, int iSrc, int iDst);
void ClOnMoveButton(DWORD dwStatus, int iPage, int iButton);
void ClOnWindowLock(DWORD dwStatus);
void ClOnRestrictionMode(DWORD dwStatus);
LRESULT CALLBACK ClAboutPluginDlg(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ClPluginSettingDlg(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ClPluginItemPropDlg(HWND, UINT, WPARAM, LPARAM);
BOOL ClPluginEventHandler(UINT uEvent, LONG_PTR lpParam1, LONG_PTR lpParam2);

//------------------------------------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------------------------------------
HINSTANCE	hInst;																// �v���O�C��DLL�̃C���X�^���X�n���h��
LPVOID		*pCpiFunc;															// CPI�֐��A�h���X�z��̃|�C���^
int			iID;																// �v���O�C��ID

