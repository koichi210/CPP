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
	constexpr int kMinYear = 1900;
	constexpr int kMaxYear = 2100;
	constexpr int kMinAge = 0;
	constexpr int kMaxAge = 130;
	constexpr int kZodiacCount = 12;

	constexpr int kDefaultAge = 20;
	constexpr int kDefaultBirth = 1980;

	constexpr const char* kAgeFormat = "満%d才";
	constexpr const char* kBirthFormat = "%d年";

	// MIN_YEAR（1900年）が子年なので、(年 - kMinYear) % 12 で引ける並び
	constexpr const char* kZodiacNames[kZodiacCount] = {
		"子(ねずみ)", "丑(うし)", "寅(とら)", "卯(うさぎ)", "辰(たつ)", "巳(み)",
		"午(うま)", "未(ひつじ)", "申(さる)", "酉(とり)", "戌(いぬ)", "亥(いのしし)" };
}

ZodiacDlg::ZodiacDlg(CWnd* parent /*=nullptr*/)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ZodiacDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
	DDX_Control(dx, IDC_YEAR, year_combo_);
	DDX_Control(dx, IDC_LIST, list_combo_);
}

BEGIN_MESSAGE_MAP(ZodiacDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AGE, &ZodiacDlg::OnAge)
	ON_BN_CLICKED(IDC_CHINEZODIAC, &ZodiacDlg::OnChineZodiac)
	ON_BN_CLICKED(IDC_VIEW, &ZodiacDlg::OnView)
	ON_BN_CLICKED(IDC_BIRTH, &ZodiacDlg::OnBirth)
	ON_CBN_SELCHANGE(IDC_YEAR, &ZodiacDlg::OnSelchangeYear)
	ON_BN_CLICKED(IDC_ALL_VIEW, &ZodiacDlg::OnAllView)
END_MESSAGE_MAP()

BOOL ZodiacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	// 年の選択肢を作り、今年を選んでおく
	time_t now = time(nullptr);
	const tm* local = localtime(&now);

	year_combo_.ResetContent();
	int idx = 0;
	for (int year = kMinYear; year <= kMaxYear; year++)
	{
		CString text;
		text.Format("%d", year);
		int ind = year_combo_.InsertString(-1, text);
		year_combo_.SetItemData(ind, year);

		if (local->tm_year + 1900 == year)
		{
			idx = ind;
		}
	}
	year_combo_.SetCurSel(idx);
	year_ = GetSelectedYear();

	CheckDlgButton(IDC_AGE, BST_CHECKED);
	OnAge();

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void ZodiacDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR ZodiacDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// CBN_SELCHANGE の時点ではコンボボックスの表示文字列がまだ前の選択のままのことがあるので、
// 選択中の項目データ（年）を読む
void ZodiacDlg::OnSelchangeYear()
{
	year_ = GetSelectedYear();
	Refresh();
}

int ZodiacDlg::GetSelectedYear() const
{
	return static_cast<int>(year_combo_.GetItemData(year_combo_.GetCurSel()));
}

void ZodiacDlg::OnBirth()
{
	ChangeMode(Mode::kBirth);
}

void ZodiacDlg::OnAge()
{
	ChangeMode(Mode::kAge);
}

void ZodiacDlg::OnChineZodiac()
{
	ChangeMode(Mode::kZodiac);
}

void ZodiacDlg::ChangeMode(Mode mode)
{
	if (mode_ != mode)
	{
		mode_ = mode;
		Refresh();
	}
}

// 表示モードに合わせて下段のリストを作り直す
void ZodiacDlg::Refresh()
{
	list_combo_.ResetContent();

	CString text;
	int current = 0;
	switch (mode_)
	{
	case Mode::kBirth:
		for (int year = kMinYear; year <= year_; year++)
		{
			text.Format(kBirthFormat, year);
			AddListItem(text, year);
		}
		current = kDefaultBirth - kMinYear;
		break;

	case Mode::kAge:
		for (int age = kMinAge; age <= kMaxAge; age++)
		{
			text.Format(kAgeFormat, age);
			AddListItem(text, age);
		}
		current = kDefaultAge - kMinAge;
		break;

	case Mode::kZodiac:
	default:
		for (int i = 0; i < kZodiacCount; i++)
		{
			AddListItem(kZodiacNames[i], i);
		}
		current = 0;
		break;
	}
	list_combo_.SetCurSel(current);
}

// 項目データには年・年齢・干支の番号を持たせ、表示時に文字列を解析しなくて済むようにする
void ZodiacDlg::AddListItem(LPCTSTR text, int data)
{
	int ind = list_combo_.InsertString(-1, text);
	list_combo_.SetItemData(ind, data);
}

void ZodiacDlg::OnView()
{
	int sel = list_combo_.GetCurSel();
	if (sel == CB_ERR)
	{
		return;
	}
	int value = static_cast<int>(list_combo_.GetItemData(sel));

	CString view;
	if (mode_ == Mode::kZodiac)
	{
		view.Format("%s年のヒト\r\n\r\n"
					"生まれた年    年齢\r\n", kZodiacNames[value]);

		for (int birth = kMinYear + value; birth < year_; birth += kZodiacCount)
		{
			view.AppendFormat(kBirthFormat, birth);
			view += "        ";
			view.AppendFormat(kAgeFormat, year_ - birth);
			view += "\r\n";
		}
	}
	else
	{
		int birth;
		int age;
		if (mode_ == Mode::kAge)
		{
			age = value;
			birth = year_ - age;
		}
		else
		{
			birth = value;
			age = year_ - birth;
		}
		CString zodiac = GetZodiac(birth);

		view.Format("%d年生まれ\r\n"
					"今年は%d才\r\n"
					"%s年です。\r\n", birth, age, static_cast<LPCTSTR>(zodiac));
	}
	SetDlgItemText(IDC_PREVIEW, view);
}

CString ZodiacDlg::GetZodiac(int year) const
{
	if (year < kMinYear || year > year_)
	{
		AfxMessageBox("プログラムエラー", MB_OK);
		return CString();
	}
	return CString(kZodiacNames[(year - kMinYear) % kZodiacCount]);
}

void ZodiacDlg::OnAllView()
{
	CString table;
	for (int i = 0; i < kZodiacCount; i++)
	{
		table.AppendFormat("%14s", kZodiacNames[i]);
	}
	table += "\r\n";

	for (int year = kMinYear; year <= year_; year++)
	{
		if ((year - kMinYear) % kZodiacCount == 0)
		{
			table += "\r\n";
		}
		table.AppendFormat("%11d年", year);
	}

	MessageBox(table, "早見表", MB_OK);
}
