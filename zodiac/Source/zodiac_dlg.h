// zodiacDlg.h : メインダイアログ（生まれた年・年齢・干支の早見）

#pragma once

class CZodiacDlg : public CDialog
{
public:
	explicit CZodiacDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_ZODIAC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAge();
	afx_msg void OnChineZodiac();
	afx_msg void OnView();
	afx_msg void OnBirth();
	afx_msg void OnSelchangeYear();
	afx_msg void OnAllView();
	DECLARE_MESSAGE_MAP()

private:
	// 下段のリストに何を並べるか
	enum class Mode { None, Birth, Age, Zodiac };

	void ChangeMode(Mode mode);
	void Refresh();
	void AddListItem(LPCTSTR text, int data);
	CString GetZodiac(int year) const;

	HICON m_hIcon;
	CComboBox m_yearCombo;
	CComboBox m_listCombo;
	Mode m_mode = Mode::None;
	int m_year = 0;		// 閲覧基準の年（今年）
};
