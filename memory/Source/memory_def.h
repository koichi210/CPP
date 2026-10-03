// memory_def.h : 定数と型

#ifndef MEMORY_SOURCE_MEMORY_DEF_H_
#define MEMORY_SOURCE_MEMORY_DEF_H_

constexpr UINT_PTR kGenerateId	= 1;	// 出題タイマのID

// 出題文字の表示フォント
constexpr int kShowWeight	= 40;
constexpr int kShowHeight	= 40;

// 最大最小値
constexpr int kMinVal			= 1;	// 各値の最小値
constexpr int kPrNumMax		= 20;	// 出題数の最大値
constexpr int kKetaAnkiMax	= 15;	// 出題桁数の最大値（暗記）
constexpr int kKetaKeisanMax	= 3;	// 出題桁数の最大値（計算）
constexpr int kMaxCyc			= 5000;	// 最大周期
constexpr int kMinCyc			= 10;	// 最小周期

// 初期値
constexpr int kCycInitVal	= 600;	// 表示周期(ms)
constexpr int kNumInitVal	= 5;	// 出題数の初期値
constexpr int kKetaInitVal	= 1;	// 出題桁数の初期値

// 状態
enum class PlayState
{
	kInit,	// 初期
	kPlaying,	// 真っ最中
	kEnd,	// 終了
};

// モード
enum class PlayMode
{
	kAnki,	// 暗記
	kKeisan,	// 計算
};

constexpr PlayMode kModeInitVal = PlayMode::kKeisan;	// 出題モードの初期値

// 出題する文字の種類（ビットの組み合わせ）
constexpr int kTypeNumber		= 1;	// 数値
constexpr int kTypeEngSmall	= 2;	// アルファベット（小）
constexpr int kTypeEngLarge	= 4;	// アルファベット（大）

// 設定値チェックの結果（ビットの組み合わせ）
constexpr int kCheckOk			= 0;	// 正常
constexpr int kCheckNumErr		= 1;	// 問題数エラー
constexpr int kCheckKetaErr	= 2;	// 桁数エラー

#endif  // MEMORY_SOURCE_MEMORY_DEF_H_
