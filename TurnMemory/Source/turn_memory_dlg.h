// turn_memory_dlg.h : メインダイアログ（出題）

#ifndef TURNMEMORY_SOURCE_TURN_MEMORY_DLG_H_
#define TURNMEMORY_SOURCE_TURN_MEMORY_DLG_H_

constexpr int kCellMax	= 10;	// 1辺の最大マス数
constexpr int kCellMin	= 3;	// 1辺の最小マス数

// マス（エディット）のコントロールID。IDC_EDIT1 から CELL_MAX 個ずつ行が並ぶ
inline int CellCtrlId(int row, int col)
{
	return IDC_EDIT1 + row * kCellMax + col;
}

// size×size のマスと行・列の見出しだけを表示する
void ShowCellGrid(CWnd& dlg, int size);

class TurnMemoryDlg : public CDialog
{
public:
	TurnMemoryDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_TURNMEMORY_DIALOG };

	// 解答ダイアログが参照する出題内容（index は左上から行ごとの通し番号）
	int GetSize() const { return size_; }
	int GetAnswer(int index) const { return answer_[index]; }
	int GetInput(int index) const { return input_[index]; }

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnTimer(UINT_PTR id_event);
	DECLARE_MESSAGE_MAP()

private:
	enum class GameState
	{
		kInit,
		kStart,
		kStop,
		kEnd,
	};

	int CellCount() const { return size_ * size_; }

	void EndProc();
	void ExitProc();
	void InitProc();
	void BuildNumber();
	void Refresh();
	void ShowProc();
	void WaitProc();

	HICON		icon_;
	int			answer_[kCellMax * kCellMax] = {};	// 各マスの正解の順番
	int			input_[kCellMax * kCellMax] = {};	// 各マスに入力された順番
	int			size_ = 0;			// 1辺のマス数
	GameState	state_ = GameState::kInit;
	int			cnt_ = 1;			// 次に表示する順番
	int			wait_ = 0;			// 記憶時間の残り秒数
};

#endif  // TURNMEMORY_SOURCE_TURN_MEMORY_DLG_H_
