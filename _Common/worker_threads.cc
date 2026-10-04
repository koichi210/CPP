// worker_threads.cc : ワーカースレッドの起動と終了待ち

#include "worker_threads.h"

#include <algorithm>

// 通常は WaitAll() 済み。呼び忘れても、終了前に CWinThread を消さないよう待つ
WorkerThreads::~WorkerThreads()
{
	WaitAll();
}

BOOL WorkerThreads::Start(AFX_THREADPROC thread_proc, LPVOID param)
{
	RemoveFinished();

	// 終了後もハンドルで待てるよう、自動削除を止めてから動かす
	CWinThread* thread = AfxBeginThread(thread_proc, param, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (thread == nullptr)
	{
		return FALSE;
	}
	thread->m_bAutoDelete = FALSE;
	threads_.emplace_back(thread);
	thread->ResumeThread();
	return TRUE;
}

BOOL WorkerThreads::IsRunning()
{
	RemoveFinished();
	return !threads_.empty();
}

void WorkerThreads::WaitAll()
{
	bool quit_received = false;
	int quit_code = 0;
	std::vector<HANDLE> handles;

	while (IsRunning())
	{
		handles.clear();
		for (const auto& thread : threads_)
		{
			if (handles.size() < MAXIMUM_WAIT_OBJECTS - 1)
			{
				handles.push_back(thread->m_hThread);
			}
		}

		// 一度に待てる数に上限があるので、時間を区切って残りを確認し直す
		::MsgWaitForMultipleObjects(static_cast<DWORD>(handles.size()), handles.data(), FALSE, 100, QS_ALLINPUT);

		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 終了要求は待ち終わってから出し直す
			if (msg.message == WM_QUIT)
			{
				quit_received = true;
				quit_code = static_cast<int>(msg.wParam);
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	if (quit_received)
	{
		::PostQuitMessage(quit_code);
	}
}

void WorkerThreads::RemoveFinished()
{
	threads_.erase(
		std::remove_if(threads_.begin(), threads_.end(),
			[](const std::unique_ptr<CWinThread>& thread)
			{
				return ::WaitForSingleObject(thread->m_hThread, 0) == WAIT_OBJECT_0;
			}),
		threads_.end());
}
