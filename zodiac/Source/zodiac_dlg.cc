// zodiacDlg.cpp : メインダイアログ（生まれた年・年齢・干支の早見）

#include "stdafx.h"
#include "zodiac.h"
#include "zodiac_dlg.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int MIN_YEAR = 1900;
	constexpr int MAX_YEAR = 2100;
	constexpr int MIN_AGE = 0;
	constexpr int MAX_AGE = 130;
	constexpr int ZODIAC_COUNT = 12;

	constexpr int DEFAULT_AGE = 20;
	constexpr int DEFAULT_BIRTH = 1980;

	constexpr const char* AGE_FORMAT = "満%d才";
	constexpr const char* BIRTH_FORMAT = "%d年";

	// MIN_YEAR（1900年）が子年なので、(年 - MIN_YEAR) % 12 で引ける並び
	constexpr const char* ZODIAC_NAMES[ZODIAC_COUNT] = {
		"子(ねずみ)", "丑(うし)", "寅(とら)", "卯(うさぎ)", "辰(たつ)", "巳(み)",
		"午(うま)", "未(ひつじ)", "申(さる)", "酉(とり)", "戌(いぬ)", "亥(いのしし)" };
}

CZodiacDlg::CZodiacDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZodiacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YEAR, m_yearCombo);
	DDX_Control(pDX, IDC_LIST, m_listCombo);
}

BEGIN_MESSAGE_MAP(CZodiacDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AGE, &CZodiacDlg::OnAge)
	ON_BN_CLICKED(IDC_CHINEZODIAC, &CZodiacDlg::OnChineZodiac)
	ON_BN_CLICKED(IDC_VIEW, &CZodiacDlg::OnView)
	ON_BN_CLICKED(IDC_BIRTH, &CZodiacDlg::OnBirth)
	ON_CBN_SELCHANGE(IDC_YEAR, &CZodiacDlg::OnSelchangeYear)
	ON_BN_CLICKED(IDC_ALL_VIEW, &CZodiacDlg::OnAllView)
END_MESSAGE_MAP()

BOOL CZodiacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 年の選択肢を作り、今年を選んでおく
	time_t now = time(nullptr);
	const tm* local = localtime(&now);

	m_yearCombo.ResetContent();
	int idx = 0;
	for (int year = MIN_YEAR; year <= MAX_YEAR; year++)
	{
		CString text;
		text.Format("%d", year);
		int ind = m_yearCombo.InsertString(-1, text);
		m_yearCombo.SetItemData(ind, year);

		if (local->tm_year + 1900 == year)
		{
			idx = ind;
		}
	}
	m_yearCombo.SetCurSel(idx);

	CString yearText;
	m_yearCombo.GetWindowText(yearText);
	m_year = atoi(yearText);

	CheckDlgButton(IDC_AGE, BST_CHECKED);
	OnAge();

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CZodiacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CZodiacDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZodiacDlg::OnSelchangeYear()
{
	CString yearText;
	m_yearCombo.GetWindowText(yearText);
	m_year = atoi(yearText);
	Refresh();
}

void CZodiacDlg::OnBirth()
{
	ChangeMode(Mode::Birth);
}

void CZodiacDlg::OnAge()
{
	ChangeMode(Mode::Age);
}

void CZodiacDlg::OnChineZodiac()
{
	ChangeMode(Mode::Zodiac);
}

void CZodiacDlg::ChangeMode(Mode mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;
		Refresh();
	}
}

// 表示モードに合わせて下段のリストを作り直す
void CZodiacDlg::Refresh()
{
	m_listCombo.ResetContent();

	CString text;
	int current = 0;
	switch (m_mode)
	{
	case Mode::Birth:
		for (int year = MIN_YEAR; year <= m_year; year++)
		{
			text.Format(BIRTH_FORMAT, year);
			AddListItem(text, year);
		}
		current = DEFAULT_BIRTH - MIN_YEAR;
		break;

	case Mode::Age:
		for (int age = MIN_AGE; age <= MAX_AGE; age++)
		{
			text.Format(AGE_FORMAT, age);
			AddListItem(text, age);
		}
		current = DEFAULT_AGE - MIN_AGE;
		break;

	case Mode::Zodiac:
	default:
		for (int i = 0; i < ZODIAC_COUNT; i++)
		{
			AddListItem(ZODIAC_NAMES[i], i);
		}
		current = 0;
		break;
	}
	m_listCombo.SetCurSel(current);
}

// 項目データには年・年齢・干支の番号を持たせ、表示時に文字列を解析しなくて済むようにする
void CZodiacDlg::AddListItem(LPCTSTR text, int data)
{
	int ind = m_listCombo.InsertString(-1, text);
	m_listCombo.SetItemData(ind, data);
}

void CZodiacDlg::OnView()
{
	int sel = m_listCombo.GetCurSel();
	if (sel == CB_ERR)
	{
		return;
	}
	int value = static_cast<int>(m_listCombo.GetItemData(sel));

	CString yearText;
	m_yearCombo.GetWindowText(yearText);
	int year = atoi(yearText);

	CString view;
	if (m_mode == Mode::Zodiac)
	{
		view.Format("%s年のヒト\r\n\r\n"
					"生まれた年    年齢\r\n", ZODIAC_NAMES[value]);

		for (int birth = MIN_YEAR + value; birth < m_year; birth += ZODIAC_COUNT)
		{
			view.AppendFormat(BIRTH_FORMAT, birth);
			view += "        ";
			view.AppendFormat(AGE_FORMAT, m_year - birth);
			view += "\r\n";
		}
	}
	else
	{
		int birth;
		int age;
		if (m_mode == Mode::Age)
		{
			age = value;
			birth = year - age;
		}
		else
		{
			birth = value;
			age = year - birth;
		}
		CString zodiac = GetZodiac(birth);

		view.Format("%d年生まれ\r\n"
					"今年は%d才\r\n"
					"%s年です。\r\n", birth, age, static_cast<LPCTSTR>(zodiac));
	}
	SetDlgItemText(IDC_PREVIEW, view);
}

CString CZodiacDlg::GetZodiac(int year) const
{
	if (year < MIN_YEAR || year > m_year)
	{
		AfxMessageBox("プログラムエラー", MB_OK);
		return CString();
	}
	return CString(ZODIAC_NAMES[(year - MIN_YEAR) % ZODIAC_COUNT]);
}

void CZodiacDlg::OnAllView()
{
	CString table;
	for (int i = 0; i < ZODIAC_COUNT; i++)
	{
		table.AppendFormat("%14s", ZODIAC_NAMES[i]);
	}
	table += "\r\n";

	for (int year = MIN_YEAR; year <= m_year; year++)
	{
		if ((year - MIN_YEAR) % ZODIAC_COUNT == 0)
		{
			table += "\r\n";
		}
		table.AppendFormat("%11d年", year);
	}

	MessageBox(table, "早見表", MB_OK);
}
