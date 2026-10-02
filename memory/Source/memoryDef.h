// memoryDef.h : 定数と型

#pragma once

constexpr UINT_PTR GENERATE_ID	= 1;	// 出題タイマのID

// 出題文字の表示フォント
constexpr int SHOW_WEIGHT	= 40;
constexpr int SHOW_HEIGHT	= 40;

// 最大最小値
constexpr int MIN_VAL			= 1;	// 各値の最小値
constexpr int PR_NUM_MAX		= 20;	// 出題数の最大値
constexpr int KETA_ANIKI_MAX	= 15;	// 出題桁数の最大値（暗記）
constexpr int KETA_KEISAN_MAX	= 3;	// 出題桁数の最大値（計算）
constexpr int MAX_CYC			= 5000;	// 最大周期
constexpr int MIN_CYC			= 10;	// 最小周期

// 初期値
constexpr int CYC_INIT_VAL	= 600;	// 表示周期(ms)
constexpr int NUM_INIT_VAL	= 5;	// 出題数の初期値
constexpr int KETA_INIT_VAL	= 1;	// 出題桁数の初期値

// 状態
enum class PlayState
{
	Init,	// 初期
	Playing,	// 真っ最中
	End,	// 終了
};

// モード
enum class PlayMode
{
	Anki,	// 暗記
	Keisan,	// 計算
};

constexpr PlayMode MODE_INIT_VAL = PlayMode::Keisan;	// 出題モードの初期値

// 出題する文字の種類（ビットの組み合わせ）
constexpr int TYPE_NUMBER		= 1;	// 数値
constexpr int TYPE_ENG_SMALL	= 2;	// アルファベット（小）
constexpr int TYPE_ENG_LARGE	= 4;	// アルファベット（大）

// 設定値チェックの結果（ビットの組み合わせ）
constexpr int CHECK_OK			= 0;	// 正常
constexpr int CHECK_NUM_ERR		= 1;	// 問題数エラー
constexpr int CHECK_KETA_ERR	= 2;	// 桁数エラー
