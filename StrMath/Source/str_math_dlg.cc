// StrMathDlg.cpp : メインダイアログ（ひらがなで足し算／引き算）

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
	constexpr LONG FONT_WEIGHT = 16;
	constexpr LONG FONT_HEIGHT = FONT_WEIGHT;

	// 百の位の読み。さんびゃく（連濁）・ろっぴゃく／はっぴゃく（促音）があるので桁ごとに持つ
	constexpr const char* HUNDREDS_READING[10] = {
		"", "ひゃく", "にひゃく", "さんびゃく", "よんひゃく",
		"ごひゃく", "ろっぴゃく", "ななひゃく", "はっぴゃく", "きゅうひゃく" };

	constexpr const char* TENS_READING[10] = {
		"", "じゅう", "にじゅう", "さんじゅう", "よんじゅう",
		"ごじゅう", "ろくじゅう", "ななじゅう", "はちじゅう", "きゅうじゅう" };

	// 一の位の 0 は読まない
	constexpr const char* ONES_READING[10] = {
		"", "いち", "に", "さん", "よん", "ご", "ろく", "なな", "はち", "きゅう" };

	// 0〜999 をひらがなの読みにする
	CString NumberToHiragana(int num)
	{
		CString str;
		str += HUNDREDS_READING[num / 100];
		str += TENS_READING[num % 100 / 10];
		str += ONES_READING[num % 10];
		return str;
	}
}

CStrMathDlg::CStrMathDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStrMathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStrMathDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CStrMathDlg::OnStart)
	ON_BN_CLICKED(IDC_SUM, &CStrMathDlg::OnSum)
	ON_BN_CLICKED(IDC_SUB, &CStrMathDlg::OnSub)
	ON_BN_CLICKED(IDC_2KETA, &CStrMathDlg::On2keta)
	ON_BN_CLICKED(IDC_3KETA, &CStrMathDlg::On3keta)
	ON_BN_CLICKED(IDC_ANS, &CStrMathDlg::OnAns)
	ON_BN_CLICKED(IDC_HLP, &CStrMathDlg::OnHlp)
END_MESSAGE_MAP()

BOOL CStrMathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CheckDlgButton(IDC_2KETA, BST_CHECKED);
	CheckDlgButton(IDC_SUM, BST_CHECKED);
	m_digits = 2;
	m_operation = Operation::Sum;
	m_num1 = 0;
	m_num2 = 0;
	srand(static_cast<unsigned>(time(nullptr)));

	LOGFONT viewFont = {};
	viewFont.lfCharSet = DEFAULT_CHARSET;
	viewFont.lfWeight = FONT_WEIGHT;
	viewFont.lfHeight = FONT_HEIGHT;
	m_font.CreateFontIndirect(&viewFont);
	GetDlgItem(IDC_VALUE1)->SetFont(&m_font);
	GetDlgItem(IDC_VALUE2)->SetFont(&m_font);
	GetDlgItem(IDC_MARK)->SetFont(&m_font);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CStrMathDlg::OnPaint()
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

HCURSOR CStrMathDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStrMathDlg::OnStart()
{
	SetDlgItemText(IDC_MARK, (m_operation == Operation::Sum) ? "+" : "-");
	SetDlgItemText(IDC_IANS, "");

	m_num1 = BuildNumber();
	m_num2 = BuildNumber();

	// 引き算の答えが負にならないよう、大きい方を先にする
	if (m_operation == Operation::Sub && m_num1 < m_num2)
	{
		std::swap(m_num1, m_num2);
	}

	SetDlgItemText(IDC_VALUE1, NumberToHiragana(m_num1));
	SetDlgItemText(IDC_VALUE2, NumberToHiragana(m_num2));
}

int CStrMathDlg::BuildNumber() const
{
	// 下位桁だけを使うので、桁数に満たない小さい乱数は引き直す
	int num;
	do
	{
		num = rand();
	} while (num < 1000);

	if (m_digits == 2)
	{
		num %= 100;
	}
	else if (m_digits == 3)
	{
		num %= 1000;
	}
	return num;
}

void CStrMathDlg::OnSum()
{
	m_operation = Operation::Sum;
}

void CStrMathDlg::OnSub()
{
	m_operation = Operation::Sub;
}

void CStrMathDlg::On2keta()
{
	m_digits = 2;
}

void CStrMathDlg::On3keta()
{
	m_digits = 3;
}

void CStrMathDlg::OnAns()
{
	if (m_num1 && m_num2)
	{
		int ans = (m_operation == Operation::Sum) ? m_num1 + m_num2 : m_num1 - m_num2;
		int input = GetDlgItemInt(IDC_IANS, nullptr, FALSE);

		MessageBox((ans == input) ? "正解！！" : "残念。。", "解答", MB_OK);
	}
	else
	{
		MessageBox("スタートを押して下さい。", "Caution", MB_OK);
	}
}

void CStrMathDlg::OnHlp()
{
	MessageBox("ひらがなで足し算／引き算をします\n\n"
		"1 出題桁数を選択\n"
		"2 演算種別を選択\n"
		"3 スタート釦押下により出題される\n"
		"これにより脳が鍛えられます！！！\n", "ヘルプ", MB_OK);
}
