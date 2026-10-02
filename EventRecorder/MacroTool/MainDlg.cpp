// MainDlg.cpp : メインダイアログ（マクロの実行・停止と設定画面の呼び出し）

#include "stdafx.h"
#include "MacroTool.h"
#include "MainDlg.h"
#include "MacroToolDlg.h"
#include "InputSimulator.h"
#include "CommonUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr UINT WM_PLAY_FINISHED = WM_USER + 100;	// 実行スレッドの終了通知

	constexpr UINT DEFAULT_REPEAT_COUNT = 1;
	constexpr UINT DEFAULT_REPEAT_DELAY_MSEC = 100;
	constexpr DWORD START_WAIT_MSEC = 1000;				// 「実行」を押してから動き出すまで
	constexpr TCHAR DEFAULT_SETTING_FILE[] = _T("save.txt");
	constexpr TCHAR START_TITLE[] = _T("[開始]");

	// 待ち時間の間、1秒ごとに押すキー（画面のロック防止用。VK_CAPITAL 等を指定する）
	constexpr int FLICKER_KEY = VK_NONE;

	// 実行スレッドに渡す設定（スレッド側で解放する）
	struct PLAYSETTINGS
	{
		HWND						hWnd;
		const std::atomic<bool>*	pRunning;
		UINT						repeatCount;
		UINT						repeatDelayMsec;
		std::vector<MACROEVENT>		events;
	};

	// 1秒ずつ待ちながら停止要求を確かめる
	void SleepWithFlicker(int sleepMsec, const std::atomic<bool>& running)
	{
		// 1秒未満の端数を先に待つ
		Sleep(sleepMsec % 1000);

		int count = 0;
		for (int i = 0; i < sleepMsec / 1000 && running; i++)
		{
			CInputSimulator::FunctionKeyAction(static_cast<BYTE>(FLICKER_KEY));
			count++;
			Sleep(1000);
		}

		// トグルするキーを元の状態に戻す
		if (count % 2)
		{
			CInputSimulator::FunctionKeyAction(static_cast<BYTE>(FLICKER_KEY));
		}
	}

	void PlayMouse(const MACROMOUSE& mouse)
	{
		CInputSimulator::MouseMove(mouse.pt);
		switch (mouse.operation)
		{
		default:
		case MOUSEOP_LCLICK:	CInputSimulator::MouseLButtonClick();	break;
		case MOUSEOP_LDOWN:		CInputSimulator::MouseLButtonDown();	break;
		case MOUSEOP_LUP:		CInputSimulator::MouseLButtonUp();		break;
		case MOUSEOP_RCLICK:	CInputSimulator::MouseRButtonClick();	break;
		case MOUSEOP_RDOWN:		CInputSimulator::MouseRButtonDown();	break;
		case MOUSEOP_RUP:		CInputSimulator::MouseRButtonUp();		break;
		case MOUSEOP_MOVE:												break;
		}
	}

	void PlayKey(const MACROKEY& key)
	{
		// 修飾キーを押したまま入力する
		if (key.modifiers & MODIFIER_SHIFT)	CInputSimulator::KeyAction(VK_SHIFT, TRUE);
		if (key.modifiers & MODIFIER_CTRL)	CInputSimulator::KeyAction(VK_CONTROL, TRUE);
		if (key.modifiers & MODIFIER_ALT)	CInputSimulator::KeyAction(VK_MENU, TRUE);

		if (KEYKIND_F1 <= key.keyKind && key.keyKind <= KEYKIND_F12)
		{
			CInputSimulator::FunctionKeyAction(static_cast<BYTE>(VK_F1 + key.keyKind - KEYKIND_F1));
		}
		else
		{
			for (const char* p = key.text; *p != '\0'; p++)
			{
				CInputSimulator::KeyAction(static_cast<WORD>(*p));
			}
		}

		if (key.modifiers & MODIFIER_SHIFT)	CInputSimulator::KeyAction(VK_SHIFT, FALSE);
		if (key.modifiers & MODIFIER_CTRL)	CInputSimulator::KeyAction(VK_CONTROL, FALSE);
		if (key.modifiers & MODIFIER_ALT)	CInputSimulator::KeyAction(VK_MENU, FALSE);
	}

	UINT PlayThreadProc(LPVOID pParam)
	{
		std::unique_ptr<PLAYSETTINGS> pSettings(static_cast<PLAYSETTINGS*>(pParam));
		const std::atomic<bool>& running = *pSettings->pRunning;

		for (UINT i = 0; i < pSettings->repeatCount && running; i++)
		{
			for (int j = 0; j < MAX_EVENT_COUNT && running; j++)
			{
				const MACROEVENT& ev = pSettings->events[j];
				if (ev.kind == EventKind::None)
				{
					break;	// これ以降は未設定
				}

				for (int k = 0; k < ev.execCount; k++)
				{
					// タイトルに「何周目:何行目:何回目」を出す
					CString title;
					title.Format(_T("[(%d:%d:%d)]"), i + 1, j + 1, k + 1);
					::SetWindowText(pSettings->hWnd, title);

					// 待ってから、動かす前に停止要求を確かめる
					SleepWithFlicker(static_cast<int>(ev.sleepMsec), running);
					if (!running)
					{
						break;
					}

					if (ev.kind == EventKind::Mouse)
					{
						PlayMouse(ev.mouse);
					}
					else
					{
						PlayKey(ev.key);
					}
				}
			}
			SleepWithFlicker(static_cast<int>(pSettings->repeatDelayMsec), running);
		}
		::PostMessage(pSettings->hWnd, WM_PLAY_FINISHED, 0, 0);

		return TRUE;
	}
}

CMainDlg::CMainDlg(CWnd* pParent)
	: CDialog(IDD, pParent)
	, m_events(MAX_EVENT_COUNT)
	, m_repeatCount(DEFAULT_REPEAT_COUNT)
	, m_repeatDelayMsec(DEFAULT_REPEAT_DELAY_MSEC)
{
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDBT_SETTING, &CMainDlg::OnSetting)
	ON_BN_CLICKED(IDBT_EXEC, &CMainDlg::OnExec)
	ON_BN_CLICKED(IDBT_STOP, &CMainDlg::OnStop)
	ON_BN_CLICKED(IDCLOSE, &CMainDlg::OnBnClickedClose)
	ON_MESSAGE(WM_PLAY_FINISHED, &CMainDlg::OnPlayFinished)
END_MESSAGE_MAP()

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_version.LoadString(IDS_VERSION);
	SetWindowText(m_version);

	return TRUE;
}

void CMainDlg::OnSetting()
{
	// 設定ファイルはカレントディレクトリに置く
	CString name;
	GetDlgItemText(IDET_SAMPLE, name);
	CString path(_T(".\\"));
	path += name.IsEmpty() ? CString(DEFAULT_SETTING_FILE) : name;

	CMacroToolDlg dlg(this, path, m_events, m_repeatCount, m_repeatDelayMsec);
	if (dlg.DoModal() == IDOK)
	{
		m_events = dlg.GetEvents();

		// 拡張子を除いたファイル名を表示する
		CString fileTitle;
		SplitPath(dlg.GetFileName(), nullptr, nullptr, &fileTitle, nullptr);
		SetDlgItemText(IDET_SAMPLE, fileTitle);
	}
}

void CMainDlg::OnExec()
{
	GetDlgItem(IDBT_EXEC)->EnableWindow(FALSE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(FALSE);

	CString title;
	title.Format(_T("%s %s"), static_cast<LPCTSTR>(m_version), START_TITLE);
	SetWindowText(title);

	Sleep(START_WAIT_MSEC);

	auto pSettings = std::make_unique<PLAYSETTINGS>();
	pSettings->hWnd = m_hWnd;
	pSettings->pRunning = &m_running;
	pSettings->repeatCount = m_repeatCount;
	pSettings->repeatDelayMsec = m_repeatDelayMsec;
	pSettings->events = m_events;

	m_running = true;
	AfxBeginThread(PlayThreadProc, pSettings.release());
}

LRESULT CMainDlg::OnPlayFinished(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	OnStop();
	ShowWindow(SW_RESTORE);
	return TRUE;
}

void CMainDlg::OnStop()
{
	if (m_running)
	{
		SetWindowText(m_version);
		m_running = false;
	}

	GetDlgItem(IDBT_EXEC)->EnableWindow(TRUE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(TRUE);
}

void CMainDlg::OnBnClickedClose()
{
	m_running = false;
	OnOK();
}
