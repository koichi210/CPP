// worker_threads.cc : ワーカースレッドの起動と終了待ち

#include "worker_threads.h"

#include <algorithm>

// 通常は WaitAll() 済み。呼び忘れても、終了前に CWinThread を消さないよう待つ
CWorkerThreads::~CWorkerThreads()
{
	WaitAll();
}

BOOL CWorkerThreads::Start(AFX_THREADPROC pfnThreadProc, LPVOID pParam)
{
	RemoveFinished();

	// 終了後もハンドルで待てるよう、自動削除を止めてから動かす
	CWinThread* pThread = AfxBeginThread(pfnThreadProc, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (pThread == nullptr)
	{
		return FALSE;
	}
	pThread->m_bAutoDelete = FALSE;
	m_threads.emplace_back(pThread);
	pThread->ResumeThread();
	return TRUE;
}

BOOL CWorkerThreads::IsRunning()
{
	RemoveFinished();
	return !m_threads.empty();
}

void CWorkerThreads::WaitAll()
{
	bool quitReceived = false;
	int quitCode = 0;

	while (IsRunning())
	{
		std::vector<HANDLE> handles;
		for (const auto& thread : m_threads)
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
				quitReceived = true;
				quitCode = static_cast<int>(msg.wParam);
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	if (quitReceived)
	{
		::PostQuitMessage(quitCode);
	}
}

void CWorkerThreads::RemoveFinished()
{
	m_threads.erase(
		std::remove_if(m_threads.begin(), m_threads.end(),
			[](const std::unique_ptr<CWinThread>& thread)
			{
				return ::WaitForSingleObject(thread->m_hThread, 0) == WAIT_OBJECT_0;
			}),
		m_threads.end());
}
