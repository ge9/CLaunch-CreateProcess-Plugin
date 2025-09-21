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
// 説明 : プラグイン名文字列を返す
//      :
// 引数 : pszTitle  プラグイン名文字列取得バッファ
//      :
// 戻値 : なし
//==============================================================================
void ClGetPluginTitle(LPTSTR pszTitle)
{
	LoadString(hInst, IDS_TITLE, pszTitle, MAX_PLUGIN_TITLE);					// 文字列リソース取得
}


//==============================================================================
// 説明 : プラグイン初期化処理
//      :
// 引数 : id      プラグイン登録ID (-1:設定ダイアログBOX用の一時登録)
//		: pvFunc  CPI関数アドレステーブルのポインタ
//      :
// 戻値 : TRUE:正常終了  FALSE:異常終了
//==============================================================================
BOOL ClPluginInit(int id, LPVOID *pvFunc)
{
	pCpiFunc = pvFunc;															// CPI関数アドレステーブルのポインタ保存
	if (CpiGetVersion() < SUPPORT_VERSION)										// 本体バージョンが古い？
	{
		return(FALSE);															// 初期化処理中止
	}
	// 設定を読み込む必要がある場合にはここに記述

	if (id < 0)																	// CLaunchオプションダイアログBOX用の一時登録？
	{
		return(TRUE);															// ここで終了
	}
	//--------------------------------------------------------------------------
	// これ以降は実際にプラグイン登録された場合の処理（一時登録ではない）
	//--------------------------------------------------------------------------
	iID = id;																	// プラグイン登録ID保存
	// ↓ボタン登録可能なプラグインの場合はここでアイテム登録を行う
	TCHAR	szName[MAX_PLUGIN_TITLE];
	LoadString(hInst, IDS_ITEM_NAME, szName, BufLen(szName));					// アイテム名文字列取得
	CpiRegisterButtonItem(iID, szName);										// ボタンアイテムに登録
	return(TRUE);																// 正常終了
}


//==============================================================================
// 説明 : プラグイン終了処理
//      :
// 引数 : id  プラグイン登録ID (-1:設定ダイアログBOX用の一時登録解除)
//      :
// 戻値 : TRUE:正常終了  FALSE:異常終了
//==============================================================================
BOOL ClPluginExit(int id)
{

	return(TRUE);																// 正常終了
}


//==============================================================================
// 説明 : プラグインアイテムデータ初期化処理
//		: ・プラグインアイテムがボタンに登録された後、呼び出されます。
//      :
// 引数 : pBI  初期化対象ボタン情報のポインタ
//      :
// 戻値 : TRUE:正常終了  FALSE:異常終了
//==============================================================================
BOOL ClPluginInitItem(LPBTNINFO pBI)
{
	// 必要に応じて以下のデータに初期値をセット
	//	lstrcpy(pBI->szParam, ...
	//	lstrcpy(pBI->szDir, ...
	//	pBI->iSW = ...
	pBI->uFlags = 6;//「通常」優先度、コンソールウインドウは通常
	return(TRUE);																// 正常終了
}


//==============================================================================
// 説明 : プラグインアイテム実行処理
//      :
// 引数 : pBI      対象アイテムボタン情報のポインタ
//		: uOption  起動オプションフラグ（0x00000001:ボタン長押しで起動）
//      :
// 戻値 : TRUE:正常終了  FALSE:異常終了
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
		MessageBox(0, L"起動に失敗しました。コマンドと作業ディレクトリを確認してください。", NULL, 0);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (uOption & EXE_PRESS_HOLD)												// ボタン長押しで起動された？
	{
		// ボタン長押しの場合の処理
	}
	else																		// 長押しではない
	{
		// 通常起動された場合の処理
	}
	return(TRUE);																// 正常終了
}


//==============================================================================
// 説明 : CLaunchが表示された時の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//      :
// 戻値 : なし
//==============================================================================
void ClOnShowWindow(DWORD dwStatus)
{

}


//==============================================================================
// 説明 : CLaunchが非表示になった時の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//      :
// 戻値 : なし
//==============================================================================
void ClOnHideWindow(DWORD dwStatus)
{

}


//==============================================================================
// 説明 : アイテムが起動される前の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     起動アイテムを含むページの番号
//		: iButton   起動アイテムのボタン番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnPreExecute(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// 説明 : アイテムが起動された後の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     起動アイテムを含むページの番号
//		: iButton   起動アイテムのボタン番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnExecute(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// 説明 : CLaunchのページが切り替わった時の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     新しく表示されたページの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnPageChange(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// 説明 : ホバー状態のタブが切り替わった時の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     ホバー状態になったページの番号 (-1:ホバー状態OFF)
//      :
// 戻値 : なし
//==============================================================================
void ClOnTabHover(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// 説明 : ホバー状態のボタンが切り替わった時の処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iBtnPos   ホバー状態になったボタンの位置番号 (-1:ホバー状態OFF)
//      :
// 戻値 : なし
//==============================================================================
void ClOnButtonHover(DWORD dwStatus, int iBtnPos)
{

}


//==============================================================================
// 説明 : ページ追加時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     追加されたページの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnAddPage(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// 説明 : ページ削除時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     削除されるページの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnDelPage(DWORD dwStatus, int iPage)
{

}


//==============================================================================
// 説明 : ボタン追加時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     ボタンが追加されたページの番号
//		: iButton   追加されたボタンの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnAddButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// 説明 : ボタン削除時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     削除されるボタンを含むページの番号
//		: iButton   削除されるボタンの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnDelButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// 説明 : ページ移動時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iSrc      移動元ページ位置
//		: iDst      移動先ページ位置
//      :
// 戻値 : なし
//==============================================================================
void ClOnMovePage(DWORD dwStatus, int iSrc, int iDst)
{

}


//==============================================================================
// 説明 : ボタン移動時処理（ページ外へのボタン移動のみ）
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//		: iPage     移動元ページの番号
//		: iButton   移動元ボタンの番号
//      :
// 戻値 : なし
//==============================================================================
void ClOnMoveButton(DWORD dwStatus, int iPage, int iButton)
{

}


//==============================================================================
// 説明 : ウィンドウロック切替え時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//      :
// 戻値 : なし
//==============================================================================
void ClOnWindowLock(DWORD dwStatus)
{
	if (dwStatus & CS_PINNED)													// ウィンドウロック？
	{
		// ウィンドウロック状態になった時の処理
	}
	else																		// ウィンドウロック解除
	{
		// ウィンドウロックが解除された時の処理
	}
}


//==============================================================================
// 説明 : 制限モード切替え時処理
//      :
// 引数 : dwStatus  CLaunchステータスフラグ
//      :
// 戻値 : なし
//==============================================================================
void ClOnRestrictionMode(DWORD dwStatus)
{
	if (dwStatus & CS_LIMITMODE)												// 制限モード中？
	{
		// 制限モードに切り替わった時の処理
	}
	else																		// 制限モード解除
	{
		// 制限モードが解除された時の処理
	}
}


//==============================================================================
// 説明 : プラグインアイテムのプロパティダイアログプロシージャ
//      :
// 引数 : hDlg    ウィンドウハンドル
//		: uMsg    メッセージ
//		: wParam  wParam
//		: lParam  lParam
//      :
// 戻値 : 処理結果
//==============================================================================
LRESULT CALLBACK ClPluginItemPropDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LPBTNINFO pBI = NULL;

	switch (uMsg)
	{
		//----------------------------------------------------------------------
		// WM_INITDIALOG：ダイアログ初期化処理
		//----------------------------------------------------------------------
	case WM_INITDIALOG:
		pBI = (LPBTNINFO)lParam;    // パラメータのボタン情報ポインタを保存

		if (pBI)
		{
			// szParam の内容をテキストボックスにセット
			SetDlgItemText(hDlg, IDC_EDIT_CMD, pBI->szParam);
			SetDlgItemText(hDlg, IDC_EDIT_WD, pBI->szDir);
			CheckDlgButton(hDlg, IDC_CHECK_CREATECONSOLE, pBI->uFlags & 0x1);

			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"リアルタイム");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"高");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"通常以上");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"通常");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"通常以下");
			SendDlgItemMessage(hDlg, IDC_COMBO_PRIORITY, CB_ADDSTRING, 0, (LPARAM)L"低");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_COMBO_PRIORITY), (pBI->uFlags & 0b1110) >> 1);
		}

		return TRUE;    // コントロールにフォーカスをセットしていない

		//----------------------------------------------------------------------
		// WM_COMMAND：
		//----------------------------------------------------------------------
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:   // OKボタンが押された
			if (pBI != NULL)
			{
				// テキストボックスの内容を取得して szParam に格納
				GetDlgItemText(hDlg, IDC_EDIT_CMD, pBI->szParam, ARRAYSIZE(pBI->szParam));
				GetDlgItemText(hDlg, IDC_EDIT_WD, pBI->szDir, ARRAYSIZE(pBI->szDir));
				int sel = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_COMBO_PRIORITY));
				UINT createConsole = IsDlgButtonChecked(hDlg, IDC_CHECK_CREATECONSOLE);
				pBI->uFlags = (sel << 1) | createConsole;
			}
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:  // キャンセルボタンが押された
			EndDialog(hDlg, FALSE);
			break;
		}
		return TRUE;    // メッセージ処理済み
	}
	return FALSE;   // メッセージ未処理
}


//==============================================================================
// 説明 : プラグイン設定ダイアログプロシージャ
//      :
// 引数 : hDlg    ウィンドウハンドル
//		: uMsg    メッセージ
//		: wParam  wParam
//		: lParam  lParam
//      :
// 戻値 : 処理結果
//==============================================================================
LRESULT CALLBACK ClPluginSettingDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//--------------------------------------------------------------------------
	//	WM_INITDIALOG：ダイアログ初期化処理
	//--------------------------------------------------------------------------
	  case WM_INITDIALOG:
		return(TRUE);															// コントロールにフォーカスをセットしていない

	//--------------------------------------------------------------------------
	//	WM_COMMAND：
	//--------------------------------------------------------------------------
	  case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		  case IDOK:															// OKボタンが押された？
			EndDialog(hDlg, TRUE);												// ダイアログBOXを閉じる
			break;

		  case IDCANCEL:														// キャンセルボタンが押された？
			EndDialog(hDlg, FALSE);												// ダイアログBOXを閉じる
			break;
		}
		return(TRUE);															// メッセージ処理済み
	}
	return(FALSE);																// メッセージ未処理
}


//==============================================================================
// 説明 : バージョン情報ダイアログプロシージャ
//      :
// 引数 : hDlg    ウィンドウハンドル
//		: uMsg    メッセージ
//		: wParam  wParam
//		: lParam  lParam
//      :
// 戻値 : 処理結果
//==============================================================================
LRESULT CALLBACK ClAboutPluginDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR	szName[MAX_PLUGIN_TITLE+1];

	switch (uMsg)
	{
	//--------------------------------------------------------------------------
	//	WM_INITDIALOG：ダイアログ初期化処理
	//--------------------------------------------------------------------------
	  case WM_INITDIALOG:
		ClGetPluginTitle(szName);												// プラグイン名取得
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC1), szName);					// スタティックコントロール１にセット
		LoadString(hInst, IDS_COPYRIGHT, szName, BufLen(szName));				// 著作情報文字列取得
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC2), szName);					// スタティックコントロール２にセット
		return(TRUE);															// コントロールにフォーカスをセットしていない

	//--------------------------------------------------------------------------
	//	WM_COMMAND：
	//--------------------------------------------------------------------------
	  case WM_COMMAND:
		if (LOWORD(wParam) == IDOK												// OKボタンが押された？
		||	LOWORD(wParam) == IDCANCEL)											// or ESCキーが押された？
		{
			EndDialog(hDlg, FALSE);												// ダイアログBOXを閉じる
		}
		return(TRUE);															// メッセージ処理済み
	}
	return(FALSE);																// メッセージ未処理
}


//==============================================================================
// 説明 : プラグインイベントハンドラ メイン関数
//      :
// 引数 : uEvent    イベント番号
//		: lpParam1  パラメータ１
//		: lpParam2  パラメータ２
//      :
// 戻値 : TRUE:初期化成功  FALSE:初期化失敗
//==============================================================================
BOOL ClPluginEventHandler(UINT uEvent, LONG_PTR lpParam1, LONG_PTR lpParam2)
{
	switch (uEvent)																// イベント番号による処理選択
	{
	  //------------------------------------------------------------------------
	  // プラグイン名取得要求
	  //------------------------------------------------------------------------
	  case PRQ_GET_TITLE:
		ClGetPluginTitle((LPTSTR)lpParam1);										// プラグイン名文字列取得
		break;
	  //------------------------------------------------------------------------
	  // プラグイン初期化要求
	  //------------------------------------------------------------------------
	  case PRQ_INIT:
		return(ClPluginInit((int)lpParam1, (LPVOID*)lpParam2));					// プラグイン初期化処理
	  //------------------------------------------------------------------------
	  // プラグイン終了要求
	  //------------------------------------------------------------------------
	  case PRQ_EXIT:
		return(ClPluginExit((int)lpParam1));									// プラグイン終了処理
	  //------------------------------------------------------------------------
	  // プラグインID変更要求
	  //------------------------------------------------------------------------
	  case PRQ_ID_CHANGE:
		iID = (int)lpParam1;													// 新しいプラグインIDを保存
		break;
	  //------------------------------------------------------------------------
	  // プラグインアイテムデータ初期化要求
	  //------------------------------------------------------------------------
	  case PRQ_INIT_ITEM:
		return(ClPluginInitItem((LPBTNINFO)lpParam1));							// プラグインアイテムデータ初期化処理
	  //------------------------------------------------------------------------
	  // プラグインアイテム実行要求
	  //------------------------------------------------------------------------
	  case PRQ_EXECUTE:
		return(ClPluginExecute((LPBTNINFO)lpParam1, (UINT)lpParam2));			// プラグインアイテム実行処理
	  //------------------------------------------------------------------------
	  // プラグインアイテムのプロパティダイアログ表示要求
	  //------------------------------------------------------------------------
	  case PRQ_ITEM_PROP:
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_PROPERTY), (HWND)lpParam1,
							(DLGPROC)ClPluginItemPropDlg, (LPARAM)lpParam2);	// プラグイン設定ダイアログ表示
		break;
	  //------------------------------------------------------------------------
	  // プラグイン設定ダイアログ表示要求
	  //------------------------------------------------------------------------
	  case PRQ_SETTING:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTING),
								(HWND)lpParam1, (DLGPROC)ClPluginSettingDlg);	// プラグイン設定ダイアログ表示
		break;
	  //------------------------------------------------------------------------
	  // プラグインバージョン情報ダイアログ表示要求
	  //------------------------------------------------------------------------
	  case PRQ_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT),
								(HWND)lpParam1, (DLGPROC)ClAboutPluginDlg);		// バージョン情報ダイアログ表示
		break;
	  //------------------------------------------------------------------------
	  // CLaunch表示
	  //------------------------------------------------------------------------
	  case PEV_SHOW:
		ClOnShowWindow((DWORD)lpParam1);										// CLaunch表示イベント処理
		break;
	  //------------------------------------------------------------------------
	  // CLaunch非表示
	  //------------------------------------------------------------------------
	  case PEV_HIDE:
		ClOnHideWindow((DWORD)lpParam1);										// CLaunch非表示イベント処理
		break;
	  //------------------------------------------------------------------------
	  // アイテム起動前
	  //------------------------------------------------------------------------
	  case PEV_PRE_EXECUTE:
		ClOnPreExecute((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));	// アイテム起動前イベント処理
		break;
	  //------------------------------------------------------------------------
	  // アイテム起動後
	  //------------------------------------------------------------------------
	  case PEV_EXECUTE:
		ClOnExecute((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// アイテム起動後イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ページ切替え
	  //------------------------------------------------------------------------
	  case PEV_PAGE_CHANGE:
		ClOnPageChange((DWORD)lpParam1, (int)lpParam2);							// ページ切替えイベント処理
		break;
	  //------------------------------------------------------------------------
	  // タブのホバー状態変化
	  //------------------------------------------------------------------------
	  case PEV_TAB_HOVER:
		ClOnTabHover((DWORD)lpParam1, (int)lpParam2);							// タブのホバー状態変化イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ボタンのホバー状態変化
	  //------------------------------------------------------------------------
	  case PEV_BUTTON_HOVER:
		ClOnButtonHover((DWORD)lpParam1, (int)lpParam2);						// ボタンのホバー状態変化イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ページ追加
	  //------------------------------------------------------------------------
	  case PEV_ADD_PAGE:
		ClOnAddPage((DWORD)lpParam1, (int)lpParam2);							// ページ追加イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ページ削除
	  //------------------------------------------------------------------------
	  case PEV_DEL_PAGE:
		ClOnDelPage((DWORD)lpParam1, (int)lpParam2);							// ページ削除イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ボタン追加
	  //------------------------------------------------------------------------
	  case PEV_ADD_BUTTON:
		ClOnAddButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// ボタン追加イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ボタン削除
	  //------------------------------------------------------------------------
	  case PEV_DEL_BUTTON:
		ClOnDelButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// ボタン削除イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ページ移動
	  //------------------------------------------------------------------------
	  case PEV_MOVE_PAGE:
		ClOnMovePage((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));		// ページ移動イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ボタン移動（ページ外へのボタン移動のみ）
	  //------------------------------------------------------------------------
	  case PEV_MOVE_BUTTON:
		ClOnMoveButton((DWORD)lpParam1, LOWORD(lpParam2), HIWORD(lpParam2));	// ボタン移動イベント処理
		break;
	  //------------------------------------------------------------------------
	  // ウィンドウロック切替え
	  //------------------------------------------------------------------------
	  case PEV_WIN_LOCK:
		ClOnWindowLock((DWORD)lpParam1);										// ウィンドウロック切替えイベント処理
		break;
	  //------------------------------------------------------------------------
	  // 制限モード切替え
	  //------------------------------------------------------------------------
	  case PEV_RESTRICT_MODE:
		ClOnRestrictionMode((DWORD)lpParam1);									// 制限モード切替えイベント処理
		break;
	}
	return(TRUE);																// 正常終了
}


//==============================================================================
// 説明 : DLL通知エントリポイント
//		:
// 引数 : hinstDLL     DLL モジュールのハンドル
//		: dwReason     関数を呼び出す理由
//		: lpvReserved  予約済み
//      :
// 戻値 : TRUE:成功  FALSE:失敗
//==============================================================================
BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hInstDLL;														// DLLのインスタンスハンドル保存
	}
	return(TRUE);
}


