// main_dlg.cc : メインダイアログ（マクロの実行・停止と設定画面の呼び出し）

#include "stdafx.h"
#include "macro_tool.h"
#include "main_dlg.h"
#include "macro_tool_dlg.h"
#include "input_simulator.h"
#include "common_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr UINT kWmPlayFinished = WM_USER + 100;	// 実行スレッドの終了通知

	constexpr UINT kDefaultRepeatCount = 1;
	constexpr UINT kDefaultRepeatDelayMsec = 100;
	constexpr DWORD kStartWaitMsec = 1000;				// 「実行」を押してから動き出すまで
	constexpr TCHAR kDefaultSettingFile[] = _T("save.txt");
	constexpr TCHAR kStartTitle[] = _T("[開始]");

	// 待ち時間の間、1秒ごとに押すキー（画面のロック防止用。VK_CAPITAL 等を指定する）
	constexpr int kFlickerKey = kVkNone;

	// 実行スレッドに渡す設定（スレッド側で解放する）
	struct PlaySettings
	{
		HWND						wnd;
		const std::atomic<bool>*	running;
		UINT						repeat_count;
		UINT						repeat_delay_msec;
		std::vector<MacroEvent>		events;
	};

	// 1秒ずつ待ちながら停止要求を確かめる
	void SleepWithFlicker(int sleep_msec, const std::atomic<bool>& running)
	{
		// 1秒未満の端数を先に待つ
		Sleep(sleep_msec % 1000);

		int count = 0;
		for (int i = 0; i < sleep_msec / 1000 && running; i++)
		{
			InputSimulator::FunctionKeyAction(static_cast<BYTE>(kFlickerKey));
			count++;
			Sleep(1000);
		}

		// トグルするキーを元の状態に戻す
		if (count % 2)
		{
			InputSimulator::FunctionKeyAction(static_cast<BYTE>(kFlickerKey));
		}
	}

	void PlayMouse(const MacroMouse& mouse)
	{
		InputSimulator::MouseMove(mouse.pt);
		switch (mouse.operation)
		{
		default:
		case kMouseOpLClick:	InputSimulator::MouseLButtonClick();	break;
		case kMouseOpLDown:		InputSimulator::MouseLButtonDown();	break;
		case kMouseOpLUp:		InputSimulator::MouseLButtonUp();		break;
		case kMouseOpRClick:	InputSimulator::MouseRButtonClick();	break;
		case kMouseOpRDown:		InputSimulator::MouseRButtonDown();	break;
		case kMouseOpRUp:		InputSimulator::MouseRButtonUp();		break;
		case kMouseOpMove:												break;
		}
	}

	void PlayKey(const MacroKey& key)
	{
		// 修飾キーを押したまま入力する
		if (key.modifiers & kModifierShift)	InputSimulator::KeyAction(VK_SHIFT, TRUE);
		if (key.modifiers & kModifierCtrl)	InputSimulator::KeyAction(VK_CONTROL, TRUE);
		if (key.modifiers & kModifierAlt)	InputSimulator::KeyAction(VK_MENU, TRUE);

		if (kKeyKindF1 <= key.key_kind && key.key_kind <= kKeyKindF12)
		{
			InputSimulator::FunctionKeyAction(static_cast<BYTE>(VK_F1 + key.key_kind - kKeyKindF1));
		}
		else
		{
			for (const char* p = key.text; *p != '\0'; p++)
			{
				InputSimulator::KeyAction(static_cast<WORD>(*p));
			}
		}

		if (key.modifiers & kModifierShift)	InputSimulator::KeyAction(VK_SHIFT, FALSE);
		if (key.modifiers & kModifierCtrl)	InputSimulator::KeyAction(VK_CONTROL, FALSE);
		if (key.modifiers & kModifierAlt)	InputSimulator::KeyAction(VK_MENU, FALSE);
	}

	UINT PlayThreadProc(LPVOID param)
	{
		std::unique_ptr<PlaySettings> settings(static_cast<PlaySettings*>(param));
		const std::atomic<bool>& running = *settings->running;

		for (UINT i = 0; i < settings->repeat_count && running; i++)
		{
			for (int j = 0; j < kMaxEventCount && running; j++)
			{
				const MacroEvent& ev = settings->events[j];
				if (ev.kind == EventKind::kNone)
				{
					break;	// これ以降は未設定
				}

				for (int k = 0; k < ev.exec_count; k++)
				{
					// タイトルに「何周目:何行目:何回目」を出す
					CString title;
					title.Format(_T("[(%d:%d:%d)]"), i + 1, j + 1, k + 1);
					::SetWindowText(settings->wnd, title);

					// 待ってから、動かす前に停止要求を確かめる
					SleepWithFlicker(static_cast<int>(ev.sleep_msec), running);
					if (!running)
					{
						break;
					}

					if (ev.kind == EventKind::kMouse)
					{
						PlayMouse(ev.mouse);
					}
					else
					{
						PlayKey(ev.key);
					}
				}
			}
			SleepWithFlicker(static_cast<int>(settings->repeat_delay_msec), running);
		}
		::PostMessage(settings->wnd, kWmPlayFinished, 0, 0);

		return TRUE;
	}
}

MainDlg::MainDlg(CWnd* parent)
	: CDialog(IDD, parent)
	, events_(kMaxEventCount)
	, repeat_count_(kDefaultRepeatCount)
	, repeat_delay_msec_(kDefaultRepeatDelayMsec)
{
}

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_BN_CLICKED(IDBT_SETTING, &MainDlg::OnSetting)
	ON_BN_CLICKED(IDBT_EXEC, &MainDlg::OnExec)
	ON_BN_CLICKED(IDBT_STOP, &MainDlg::OnStop)
	ON_BN_CLICKED(IDCLOSE, &MainDlg::OnBnClickedClose)
	ON_MESSAGE(kWmPlayFinished, &MainDlg::OnPlayFinished)
END_MESSAGE_MAP()

BOOL MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	version_.LoadString(IDS_VERSION);
	SetWindowText(version_);

	return TRUE;
}

void MainDlg::OnSetting()
{
	// 設定ファイルはカレントディレクトリに置く
	CString name;
	GetDlgItemText(IDET_SAMPLE, name);
	CString path(_T(".\\"));
	path += name.IsEmpty() ? CString(kDefaultSettingFile) : name;

	MacroToolDlg dlg(this, path, events_, repeat_count_, repeat_delay_msec_);
	if (dlg.DoModal() == IDOK)
	{
		events_ = dlg.GetEvents();

		// 拡張子を除いたファイル名を表示する
		CString file_title;
		SplitPath(dlg.GetFileName(), nullptr, nullptr, &file_title, nullptr);
		SetDlgItemText(IDET_SAMPLE, file_title);
	}
}

void MainDlg::OnExec()
{
	GetDlgItem(IDBT_EXEC)->EnableWindow(FALSE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(FALSE);

	CString title;
	title.Format(_T("%s %s"), static_cast<LPCTSTR>(version_), kStartTitle);
	SetWindowText(title);

	Sleep(kStartWaitMsec);

	auto settings = std::make_unique<PlaySettings>();
	settings->wnd = m_hWnd;
	settings->running = &running_;
	settings->repeat_count = repeat_count_;
	settings->repeat_delay_msec = repeat_delay_msec_;
	settings->events = events_;

	running_ = true;
	AfxBeginThread(PlayThreadProc, settings.release());
}

LRESULT MainDlg::OnPlayFinished(WPARAM /*w_param*/, LPARAM /*l_param*/)
{
	OnStop();
	ShowWindow(SW_RESTORE);
	return TRUE;
}

void MainDlg::OnStop()
{
	if (running_)
	{
		SetWindowText(version_);
		running_ = false;
	}

	GetDlgItem(IDBT_EXEC)->EnableWindow(TRUE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(TRUE);
}

void MainDlg::OnBnClickedClose()
{
	running_ = false;
	OnOK();
}
