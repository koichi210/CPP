// turn_memory_dlg.h : メインダイアログ（出題）

#pragma once

constexpr int CELL_MAX	= 10;	// 1辺の最大マス数
constexpr int CELL_MIN	= 3;	// 1辺の最小マス数

// マス（エディット）のコントロールID。IDC_EDIT1 から CELL_MAX 個ずつ行が並ぶ
inline int CellCtrlId(int row, int col)
{
	return IDC_EDIT1 + row * CELL_MAX + col;
}

// size×size のマスと行・列の見出しだけを表示する
void ShowCellGrid(CWnd& dlg, int size);

class CTurnMemoryDlg : public CDialog
{
public:
	CTurnMemoryDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_TURNMEMORY_DIALOG };

	// 解答ダイアログが参照する出題内容（index は左上から行ごとの通し番号）
	int GetSize() const { return m_size; }
	int GetAnswer(int index) const { return m_answer[index]; }
	int GetInput(int index) const { return m_input[index]; }

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	enum class GameState
	{
		Init,
		Start,
		Stop,
		End,
	};

	int CellCount() const { return m_size * m_size; }

	void EndProc();
	void ExitProc();
	void InitProc();
	void BuildNumber();
	void Refresh();
	void ShowProc();
	void WaitProc();

	HICON		m_hIcon;
	int			m_answer[CELL_MAX * CELL_MAX] = {};	// 各マスの正解の順番
	int			m_input[CELL_MAX * CELL_MAX] = {};	// 各マスに入力された順番
	int			m_size = 0;			// 1辺のマス数
	GameState	m_state = GameState::Init;
	int			m_cnt = 1;			// 次に表示する順番
	int			m_wait = 0;			// 記憶時間の残り秒数
};
