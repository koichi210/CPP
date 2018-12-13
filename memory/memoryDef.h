// memoryDef.h : ヘッダー ファイル
//
#define FONT		1	// ちょっと厄介なヤツ

#define GENERATE_ID	1	//タイマID
#define SHOW_WEIGHT	40	//表示文字列の幅
#define SHOW_HEIGHT	40	//表示文字列の高さ
#define STR_BUFF	256
#define ANSER_BUFF	1024	
#define HELP_BUFF	4096	

//最大最小値
#define	MIN_VAL		1		//各値の最小値
#define PR_NUM_MAX	20		//出題数の最大値
#define KETA_ANIKI_MAX	15	//出題桁数の最大値
#define KETA_KEISAN_MAX	3	//出題桁数の最大値
#define MAX_CYC		5000	// 最大周期
#define MIN_CYC		10		// 最小周期
#define ANS_OFFSET	15		//メッセージ表示のヘッダ分
#define ANS_MAX		(KETA_ANIKI_MAX+ANS_OFFSET)

//初期値
#define	CYC_INIT_VAL	600		//表示周期(ms)
#define	NUM_INIT_VAL	5		//出題数の初期値
#define	KETA_INIT_VAL	1		//出題桁数の初期値
#define TYPE_INIT_VAL	KEISAN	// 出題種別の初期値

//状態
#define	INIT		0	//初期
#define	ING			1	//真っ最中
#define	END			2	//終了

//モード
#define	ANKI		1	//暗記
#define	KEISAN		2	//計算

//タイプ
#define NUMBER		1	//数値
#define ENG_SMALL	2	//アルファベット（小）
#define ENG_LARGE	4	//アルファベット（大）

// リターン値
#define NORMAL		0	//正常リターン
#define NUM_ERR		1	//問題数エラー
#define KETA_ERR	2	//桁数エラー


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
