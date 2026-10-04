// str_math_dlg.cc : メインダイアログ（ひらがなで足し算／引き算）

#include "stdafx.h"
#include "str_math.h"
#include "str_math_dlg.h"
#include <ctime>
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr LONG kFontWeight = 16;
	constexpr LONG kFontHeight = kFontWeight;

	// 百の位の読み。さんびゃく（連濁）・ろっぴゃく／はっぴゃく（促音）があるので桁ごとに持つ
	constexpr const char* kHundredsReading[10] = {
		"", "ひゃく", "にひゃく", "さんびゃく", "よんひゃく",
		"ごひゃく", "ろっぴゃく", "ななひゃく", "はっぴゃく", "きゅうひゃく" };

	constexpr const char* kTensReading[10] = {
		"", "じゅう", "にじゅう", "さんじゅう", "よんじゅう",
		"ごじゅう", "ろくじゅう", "ななじゅう", "はちじゅう", "きゅうじゅう" };

	// 一の位の 0 は読まない
	constexpr const char* kOnesReading[10] = {
		"", "いち", "に", "さん", "よん", "ご", "ろく", "なな", "はち", "きゅう" };

	// 0〜999 をひらがなの読みにする
	CString NumberToHiragana(int num)
	{
		// 各位の読みは 0 を空にしているので、0 そのものは別に読む
		if (num == 0)
		{
			return "ぜろ";
		}

		CString str;
		str += kHundredsReading[num / 100];
		str += kTensReading[num % 100 / 10];
		str += kOnesReading[num % 10];
		return str;
	}
}

StrMathDlg::StrMathDlg(CWnd* parent /*=nullptr*/)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void StrMathDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(StrMathDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &StrMathDlg::OnStart)
	ON_BN_CLICKED(IDC_SUM, &StrMathDlg::OnSum)
	ON_BN_CLICKED(IDC_SUB, &StrMathDlg::OnSub)
	ON_BN_CLICKED(IDC_2KETA, &StrMathDlg::On2keta)
	ON_BN_CLICKED(IDC_3KETA, &StrMathDlg::On3keta)
	ON_BN_CLICKED(IDC_ANS, &StrMathDlg::OnAns)
	ON_BN_CLICKED(IDC_HLP, &StrMathDlg::OnHlp)
END_MESSAGE_MAP()

BOOL StrMathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	CheckDlgButton(IDC_2KETA, BST_CHECKED);
	CheckDlgButton(IDC_SUM, BST_CHECKED);
	srand(static_cast<unsigned>(time(nullptr)));

	LOGFONT view_font = {};
	view_font.lfCharSet = DEFAULT_CHARSET;
	view_font.lfWeight = kFontWeight;
	view_font.lfHeight = kFontHeight;
	font_.CreateFontIndirect(&view_font);
	GetDlgItem(IDC_VALUE1)->SetFont(&font_);
	GetDlgItem(IDC_VALUE2)->SetFont(&font_);
	GetDlgItem(IDC_MARK)->SetFont(&font_);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void StrMathDlg::OnPaint()
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

HCURSOR StrMathDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void StrMathDlg::OnStart()
{
	SetDlgItemText(IDC_MARK, (operation_ == Operation::kSum) ? "+" : "-");
	SetDlgItemText(IDC_IANS, "");

	num1_ = BuildNumber();
	num2_ = BuildNumber();
	started_ = true;

	// 引き算の答えが負にならないよう、大きい方を先にする
	if (operation_ == Operation::kSub && num1_ < num2_)
	{
		std::swap(num1_, num2_);
	}

	SetDlgItemText(IDC_VALUE1, NumberToHiragana(num1_));
	SetDlgItemText(IDC_VALUE2, NumberToHiragana(num2_));
}

int StrMathDlg::BuildNumber() const
{
	// 2桁なら 0〜99、3桁なら 0〜999
	const int modulus = (digits_ == 3) ? 1000 : 100;
	return rand() % modulus;
}

void StrMathDlg::OnSum()
{
	operation_ = Operation::kSum;
}

void StrMathDlg::OnSub()
{
	operation_ = Operation::kSub;
}

void StrMathDlg::On2keta()
{
	digits_ = 2;
}

void StrMathDlg::On3keta()
{
	digits_ = 3;
}

void StrMathDlg::OnAns()
{
	if (started_)
	{
		int ans = (operation_ == Operation::kSum) ? num1_ + num2_ : num1_ - num2_;
		int input = GetDlgItemInt(IDC_IANS, nullptr, FALSE);

		MessageBox((ans == input) ? "正解！！" : "残念。。", "解答", MB_OK);
	}
	else
	{
		MessageBox("スタートを押して下さい。", "Caution", MB_OK);
	}
}

void StrMathDlg::OnHlp()
{
	MessageBox("ひらがなで足し算／引き算をします\n\n"
		"1 出題桁数を選択\n"
		"2 演算種別を選択\n"
		"3 スタート釦押下により出題される\n"
		"これにより脳が鍛えられます！！！\n", "ヘルプ", MB_OK);
}
