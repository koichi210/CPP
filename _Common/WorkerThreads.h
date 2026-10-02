// WorkerThreads.h : ワーカースレッドの起動と終了待ち

#pragma once

#include <afxwin.h>

#include <memory>
#include <vector>

// 起動したワーカースレッドを覚えておき、ウィンドウを閉じる前に終了を待つためのクラス。
// ワーカーがダイアログを触る作りのとき、閉じた後に破棄済みのダイアログを触らせないために使う。
//
// 使い方:
//   1. Start() でスレッドを起動する
//   2. 閉じるとき（OnCancel など）は、停止フラグを立ててから WaitAll() で終了を待つ
class CWorkerThreads
{
public:
	CWorkerThreads() = default;
	~CWorkerThreads();

	CWorkerThreads(const CWorkerThreads&) = delete;
	CWorkerThreads& operator=(const CWorkerThreads&) = delete;

	BOOL Start(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
	BOOL IsRunning();

	// すべてのスレッドの終了を待つ。待っている間もメッセージを処理するので、
	// ワーカーが SetWindowText などで UI スレッドに SendMessage してもデッドロックしない。
	// 呼ぶ前に、ワーカーに停止を伝えておくこと（伝えないと終わるまで待ち続ける）。
	void WaitAll();

private:
	void RemoveFinished();

	std::vector<std::unique_ptr<CWinThread>> m_threads;
};
