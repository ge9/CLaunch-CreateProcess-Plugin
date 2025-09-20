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
// CLaunch本体の対応バージョン
//	プラグインが対応可能なCLaunchの本体バージョンを設定してください
#define	SUPPORT_VERSION		0x03000000
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 制限値
//------------------------------------------------------------------------------
#define MAX_PLUGIN_TITLE	MAX_PATH											// プラグイン名最大長
#define	MAX_PLUGINNAME		30													// プラグインアイテム名
#define MAX_PAGENAME		50													// ページの名最大長
#define	MAX_BTNNAME			50													// ボタン名長さ
#define	MAX_CAPTION			100													// キャプション文字数

//------------------------------------------------------------------------------
// ウィンドウ表示位置		(CpiShowWindow()の第１パラメータ)
//------------------------------------------------------------------------------
#define	WPOS_NOMOVE			0x00000000											// 現在位置
#define	WPOS_CENTER			0x00000001											// 画面中央
#define	WPOS_MOUSE_CENTER	0x00000002											// カーソルの周囲
#define	WPOS_SPECIFY		0x00000003											// 指定座標
#define	WPOS_MOUSE_LEFTOP	0x00000004											// カーソルの右下
#define	WPOS_AUTO_HIDE		0x00000100											// 自動非表示指定フラグ
#define	WPOS_FORCE_SHOW		0x00000200											// 強制表示指定フラグ
#define	GETWINPOS(pos)		(pos & 0x000000ff)									// 表示位置番号抽出マクロ

//------------------------------------------------------------------------------
// 初期表示ページ			(CpiShowWindow()の第４パラメータ)
//------------------------------------------------------------------------------
#define	PN_MEMORY			0xffff												// 前回のページ表示

//------------------------------------------------------------------------------
// ステータスフラグ			(CpiCheckStatus()のパラメータ)
//------------------------------------------------------------------------------
#define	CS_DISPMODE			0x00000001											// 表示モード
#define	CS_PINNED			0x00000002											// ウィンドウロック中
#define	CS_LIMITMODE		0x00000004											// 制限モード中
#define	CS_SEARCHWND		0x00000008											// 検索ウィンドウ

//------------------------------------------------------------------------------
// 動作設定オプションフラグ	(CpiCheckSetting()のパラメータ)
//------------------------------------------------------------------------------
#define	OPT_CLOSE_SRCHWND	0x00000001											// アイテム起動後に検索ウィンドウを閉じる
#define	OPT_CANCEL_HOLDING	0x00000002											// カーソルが移動したら長押しをキャンセルする
#define	OPT_CNV_RELATIVE	0x00000004											// アイテムを相対パスで登録する
#define	OPT_UPDATE_TRASH	0x00000008											// ごみ箱のアイコンを定期更新する
#define	OPT_AUTO_SPC_DEL	0x00000010											// アイテム削除後に空白を自動で詰める
#define	OPT_DND_HIDE		0x00000020											// ドラッグ＆ドロップ起動後にウィンドウを非表示化
#define	OPT_USE_FILER		0x00000040											// ファイラーを使用する
#define	OPT_AUTO_BACKUP		0x00000080											// 定期的に設定をバックアップする
#define	OPT_AUTO_PAGESW		0x00000100											// タブのマウスオーバーでページを切替える
#define	OPT_CIRCLE_AUTOHIDE	0x00000200											// カーソルがウィンドウ外へ出たら非表示化(サークル)
#define	OPT_AUTO_MODECNG	0x00000400											// アイテム起動後に表示モード1へ切替える
#define	OPT_DELAY_SINGLE	0x00000800											// シングルクリック遅延実行
#define	OPT_DELAY_HOTKEY	0x00001000											// 連続キー入力のタイムアウトを待つ
#define	OPT_PAGE_LOOP		0x00002000											// ページ切替えループ
#define	OPT_PROP_AFT_REG	0x00004000											// アイテム登録後プロパティ表示
#define	OPT_EDGE_AUTOHIDE	0x00008000											// カーソルがウィンドウ外へ出たら非表示化(エッジ)
#define	OPT_HOTKEY_AUTOHIDE	0x00010000											// カーソルがウィンドウ外へ出たら非表示化(ホットキー)
#define	OPT_SEND_MDBLCLK	0x00020000											// ホイールダブルクリック時のオートスクロール防止
#define	OPT_MENU_AUTOSCRL	0x00040000											// スクロールボタンのマウスオーバーで自動スクロール(サブメニュー)
#define	OPT_CLICK_AUTOHIDE	0x00080000											// カーソルがウィンドウ外へ出たら非表示化(クリック)
#define	OPT_FREE_MEMORY		0x00100000											// 不要メモリ解放
#define	OPT_CLICK_HIPRI		0x00200000											// クリック判定処理の優先度を上げる
#define	OPT_SAVE_WINLOCK	0x00400000											// ウィンドウロック状態を記憶する
#define	OPT_SAVE_RESTRICT	0x00800000											// 制限モード状態を記憶する
#define	OPT_EDGE_PRESS		0x01000000											// マウスボタン押下中のみエッジ表示
#define	OPT_MENU_KEYCURS	0x02000000											// カーソル位置のメニューをキー操作の対象にする(サブメニュー)
#define	OPT_MENU_DELAYICON	0x04000000											// メニュー表示後にアイコンを取得する(サブメニューのフォルダー展開時)
#define	OPT_CIRCLE_HIPRI	0x08000000											// サークル判定処理の優先度を上げる
#define	OPT_ASSIGN_SKEY		0x10000000											// アイテム登録時にショートカットキーを自動で割り当てる
#define	OPT_INHSCR_KEYSEL	0x20000000											// ボタン選択操作でスクロールしない
#define	OPT_UNUSED_BIT		0xc0000000											// オプション設定フラグの未使用ビット

//------------------------------------------------------------------------------
// 表示設定オプションフラグ	(CpiCheckDispSetting()のパラメータ)
//------------------------------------------------------------------------------
#define	DSP_TOPMOST			0x00000001											// 最前面に表示する
#define	DSP_ALPHA_INACTIVE	0x00000002											// 非アクティブ時のみ透明化
#define	DSP_CAP_BTNNAME		0x00000004											// キャプションにボタン名を表示
#define	DSP_CAP_BTNTIP		0x00000008											// キャプションにツールチップ文字列を表示
#define	DSP_SHOW_TIPS		0x00000010											// ツールチップを表示する
#define	DSP_TIPS_TAB		0x00000020											// タブのツールチップ
#define	DSP_TIPS_BALLOON	0x00000040											// バルーン型ツールチップ
#define	DSP_TRAY_ICON		0x00000080											// トレイアイコンを表示する
#define	DSP_CHANGE_TICON	0x00000100											// トレイアイコンを変更する
#define	DSP_MENU_LARGEICON	0x00000200											// 大きいアイコンを使用する(サブメニュー)
#define	DSP_SHOW_MENUTIPS	0x00000400											// ツールチップを表示する(サブメニュー)
#define	DSP_WHILE_PINNED	0x00000800											// ウィンドウロック中のみ

//------------------------------------------------------------------------------
// アイテム起動時追加オプションフラグ
//------------------------------------------------------------------------------
#define	EXE_PRESS_HOLD		0x00000001											// ボタン長押し起動

//------------------------------------------------------------------------------
//	構造体宣言
//------------------------------------------------------------------------------
//･･････････････････････････････････････････････････････････････････････････････
// ページ情報構造体
//･･････････････････････････････････････････････････････････････････････････････
typedef struct {
	int			iNumBtn;														// ページ内ボタン数
	TCHAR		szPage[MAX_PAGENAME+1];											// ページ名
} PAGEINFO, *LPPAGEINFO;

//･･････････････････････････････････････････････････････････････････････････････
// ボタン情報構造体
// szParam, szDir, iSW, uFlagsにはプラグインアイテムの固有設定を保存可能
//･･････････････････････････････････････････････････････････････････････････････
typedef struct {
	int				iPage;														// 所属ページのインデックス
	int				iNumber;													// ボタンインデックス
	int				iPos;														// ボタン表示位置
	UINT			uType;														// アイテムタイプ
	TCHAR			szName[MAX_BTNNAME+1];										// ボタン名
	TCHAR			szTip[MAX_PATH];											// ツールチップ
	TCHAR			szFile[MAX_PATH];											// ファイル名
	TCHAR			szParam[MAX_PATH];											// パラメータ
	TCHAR			szDir[MAX_PATH];											// ワークディレクトリ
	int				iSW;														// ウィンドウ初期表示状態
	WORD			wKeyboard;													// ショートカットキー
	UINT			uFlags;														// 起動時オプションフラグ
} BTNINFO, *LPBTNINFO;

//------------------------------------------------------------------------------
// 汎用マクロ
//------------------------------------------------------------------------------
#define	BufLen(sz)		(sizeof(sz)/sizeof(TCHAR))								// 文字列バッファのサイズ(文字数)取得

//------------------------------------------------------------------------------
// イベント番号
//------------------------------------------------------------------------------
// 処理要求
#define PRQ_GET_TITLE		0x00000001											// プラグイン名取得要求
#define PRQ_INIT			0x00000002											// プラグイン初期化要求
#define PRQ_EXIT			0x00000003											// プラグイン終了要求
#define PRQ_ID_CHANGE		0x00000004											// プラグインID変更要求
#define PRQ_INIT_ITEM		0x00000005											// アイテムデータ初期化要求
#define PRQ_EXECUTE			0x00000006											// アイテム実行要求
#define PRQ_ABOUT			0x00000007											// プラグインバージョン情報ダイアログ表示要求
#define PRQ_SETTING			0x00000008											// プラグイン設定ダイアログ表示要求
#define PRQ_ITEM_PROP		0x00000009											// プラグインアイテムのプロパティダイアログ表示要求
// イベント
#define PEV_SHOW			0x00010001											// CLaunch表示
#define PEV_HIDE			0x00010002											// CLaunch非表示
#define PEV_PRE_EXECUTE		0x00010003											// アイテム起動前
#define PEV_EXECUTE			0x00010004											// アイテム起動後
#define PEV_PAGE_CHANGE		0x00010005											// ページ切替え
#define PEV_TAB_HOVER		0x00010006											// タブのホバー状態変化
#define PEV_BUTTON_HOVER	0x00010007											// ボタンのホバー状態変化
#define PEV_ADD_PAGE		0x00010008											// ページ追加
#define PEV_DEL_PAGE		0x00010009											// ページ削除
#define PEV_ADD_BUTTON		0x0001000a											// ボタン追加
#define PEV_DEL_BUTTON		0x0001000b											// ボタン削除
#define PEV_MOVE_PAGE		0x0001000c											// ページ移動
#define PEV_MOVE_BUTTON		0x0001000d											// ボタン移動（ページ外への移動）
#define PEV_WIN_LOCK		0x0001000e											// ウィンドウロック切替え
#define PEV_RESTRICT_MODE	0x0001000f											// 制限モード切替え

//------------------------------------------------------------------------------
// CPI関数定義
//------------------------------------------------------------------------------
// 関数タイプ宣言
typedef DWORD (*LPGET_VERSION)(void);											// CLaunchのバージョンを取得する
typedef void (*LPSHOW_WINDOW)(UINT uPos, int x, int y, int iPage);				// CLaunchを表示する
typedef void (*LPHIDE_WINDOW)(void);											// CLaunchを非表示にする
typedef void (*LPGET_HANDLE)(HINSTANCE *phInst, HWND *phWnd);					// CLaunchのインスタンスハンドル、ウィンドウハンドルを取得する
typedef int (*LPGET_PAGENUM)(void);												// ページ数を取得する
typedef int (*LPGET_CURPAGE)(void);												// 現在のページ番号を取得する
typedef LPPAGEINFO (*LPGET_PAGEINFO)(int iPage);								// ページ情報を取得する
typedef LPBTNINFO (*LPGET_BUTTONINFO)(int iPage, int iBtn);						// 指定ページ、指定ボタンのボタン情報を取得する
typedef LPBTNINFO (*LPGET_DISPBTNINFO)(int iPos);								// 表示ページ内、指定位置のボタン情報を取得する
typedef void (*LPSET_CAPTION)(int id, LPCTSTR pszCaption, BOOL bSave);			// キャプションを変更する
typedef void (*LPREG_BUTTONITEM)(int id, LPCTSTR pszRegName);					// プラグインをボタンアイテムとして登録する
typedef void (*LPCHANGE_PAGE)(int iPage);										// 指定ページに切替える
typedef void (*LPSELECT_BUTTON)(int iBtnPos);									// 指定位置のボタンを選択する
typedef void (*LPEXECUTE_ITEM)(int iPage, int iButton);							// 指定アイテムを起動する
typedef BOOL (*LPCHK_STATUS)(UINT uBit);										// CLaunchのステータを取得する
typedef BOOL (*LPCHK_SETTING)(UINT uBit);										// CLaunchの動作オプション設定を取得する
typedef BOOL (*LPCHK_DISPSETTING)(UINT uBit);									// CLaunchの表示関連オプション設定を取得する

// 関数定義
#define CpiGetVersion			((LPGET_VERSION)pCpiFunc[0])					// CLaunchのバージョンを取得する
#define CpiShowWindow			((LPSHOW_WINDOW)pCpiFunc[1])					// CLaunchを表示する
#define CpiHideWindow			((LPHIDE_WINDOW)pCpiFunc[2])					// CLaunchを非表示にする
#define CpiGetHandle			((LPGET_HANDLE)pCpiFunc[3])						// CLaunchのインスタンスハンドル、ウィンドウハンドルを取得する
#define CpiGetPageNum			((LPGET_PAGENUM)pCpiFunc[4])					// ページ数を取得する
#define CpiGetCurrentPage		((LPGET_CURPAGE)pCpiFunc[5])					// 現在のページ番号を取得する
#define CpiGetPageinfo			((LPGET_PAGEINFO)pCpiFunc[6])					// ページ情報を取得する
#define CpiGetButtonInfo		((LPGET_BUTTONINFO)pCpiFunc[7])					// 指定ページ、指定ボタンのボタン情報を取得する
#define CpiGetDispBtnInfo		((LPGET_DISPBTNINFO)pCpiFunc[8])				// 表示ページ内、指定位置のボタン情報を取得する
#define CpiSetCaption			((LPSET_CAPTION)pCpiFunc[9])					// キャプションを変更する
#define CpiRegisterButtonItem	((LPREG_BUTTONITEM)pCpiFunc[10])				// プラグインをボタンアイテムとして登録する
#define CpiChangePage			((LPCHANGE_PAGE)pCpiFunc[11])					// 指定ページに切替える
#define CpiSelectButton			((LPSELECT_BUTTON)pCpiFunc[12])					// 指定位置のボタンを選択する
#define CpiExecuteItem			((LPEXECUTE_ITEM)pCpiFunc[13])					// 指定アイテムを起動する
#define CpiCheckStatus			((LPCHK_STATUS)pCpiFunc[14])					// CLaunchのステータを取得する
#define CpiCheckSetting			((LPCHK_SETTING)pCpiFunc[15])					// CLaunchの動作オプション設定を取得する
#define CpiCheckDispSetting		((LPCHK_DISPSETTING)pCpiFunc[16])				// CLaunchの表示関連オプション設定を取得する

//------------------------------------------------------------------------------
// プラグイン開発キット内関数プロトタイプ宣言
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
// グローバル変数
//------------------------------------------------------------------------------
HINSTANCE	hInst;																// プラグインDLLのインスタンスハンドル
LPVOID		*pCpiFunc;															// CPI関数アドレス配列のポインタ
int			iID;																// プラグインID

