# _Common

MFC（MBCS / Shift-JIS）アプリ向けの共通部品。各プロジェクトにコピーせず、ここを参照して使う。

| ファイル | 内容 |
|---|---|
| `common.h` | 下の全部をまとめてインクルード |
| `common_util` | パス操作・文字列置換・半角⇔全角・フォルダ選択・ダイアログ文字列設定 |
| `common_ctrl` | `BitmapStatic` / `RestrictedEdit` / `PopupEdit` / `PopupList` / `EditableListCtrl` / `IconComboBox` / `SimpleListCtrl` |
| `input_simulator` | キーボード・マウス入力のエミュレート |
| `file_comparer` | 2ファイルのバイナリ比較 |
| `worker_threads` | ワーカースレッドの起動と、ウィンドウを閉じる前の終了待ち（メッセージを処理しながら待つのでデッドロックしない） |

## プロジェクトへの組み込み

1. 使う `.cc` をプロジェクトに追加する（例: `..\..\_Common\common_util.cc`）
2. 追加した `_Common` の `.cc` は **プリコンパイル済みヘッダーを「使用しない」** にする
   （プロジェクトごとに `stdafx.h` / `pch.h` が違うため、`_Common` 側は PCH に依存しない作り）
3. インクルードディレクトリに `_Common` を追加し、`#include "common.h"`
4. 文字セットは「マルチバイト」。ソースは UTF-8(BOM付) なので、
   C/C++ の追加オプションに `/source-charset:utf-8 /execution-charset:.932` を指定する
   （`common_util.cc` の `static_assert` で Shift-JIS 埋め込みを確認している）

## 旧 StandardTemplate からの移行表

| 旧 | 新 | 備考 |
|---|---|---|
| `MyMergePath(CString*, ...)` | `MergePath(CString&, ...)` | 区切りの `\` を重複させない |
| `MySplitPath(...)` | `SplitPath(...)` | 空の要素も出力に反映する（以前は前の値が残った） |
| `MyConnection` | `AppendPath` に統合 | |
| `AppendPath(CString*, CString)` | `AppendPath(CString&, LPCTSTR)` | 空のパスに連結しても先頭に `\` を付けない |
| `AppendExt(CString*, CString)` | `AppendExt(CString&, LPCTSTR)` | `".txt"` を渡しても `..txt` にならない |
| `ReplaceString(old, CString* out, srch, rep, bDiff)` | `CString ReplaceString(src, search, replace, case_sensitive)` | 戻り値で返す。TRUE で大文字小文字を区別（旧コメントは逆に書かれていた） |
| `Browse(hWnd, title, CString*)` | `BrowseFolder(hWnd, title, CString&)` | |
| `TABLE` / `SetDlgItemTextAll` | `DlgItemText` / `SetDlgItemTextAll` | |
| `han2zen(char*)` / `zen2han(char*)` | `CString HankakuToZenkaku(CString)` / `CString ZenkakuToHankaku(CString)` | 戻り値で返す（バッファ長の心配が不要） |
| `issjiskanji(c)` | `IsSjisLeadByte(c)` | |
| `CMyStaticImage` | `BitmapStatic` | |
| `CMyRistrictedSBCS` | `RestrictedEdit` | `F_DIGIT` 等 → `kCharDigit` 等、`SetExceptionCharacters` → `SetForbiddenCharacters` |
| `CMyEdit` | `PopupEdit` | `USESTRING_*` → `kInput*`、`STATE_OK/NG` → `kResultOk/kResultCancel` |
| `CMyList` | `PopupList` | |
| `CMyListCtrl` | `EditableListCtrl` | `SetElements` → `SetListItems`、`SetEditPossible` → `SetEditKind`。仮想関数 `CreatePopup` / `UseInEditKey` は名前そのまま |
| `WM_MYEDIT_KILLFOCUS` / `WM_MYLIST_KILLFOCUS` | `kWmPopupEditClosed` / `kWmPopupListClosed` | |
| `CMyEvent` | `InputSimulator` | メンバは static になった（インスタンス経由の呼び出しもそのまま動く） |
| `CCustomCtrl` | `SimpleListCtrl` | `SetLabel` → `AddColumn`、`SetMaxColumnNum` → `SetRowCount`、`FillRect` → `FillColumn`、`SelectIdx` → `SelectRow`、`EnableSelectRowAll` → `EnableFullRowSelect`、`SetMaxRowNum` は未使用のため削除 |
| `CMyCompareFile` | `FileComparer` | `m_err` → `GetError()` |
| `Trim` / `GetArgument` / `GetArgumentSample` | 削除 | 未使用。`Trim` は `CString::Trim(LPCTSTR)` で代替可 |
| `USE_IMM_H`（IME 無効化） | 削除 | 既定で無効のまま、有効化しているプロジェクトが無かった |

## Google スタイル移行表（旧名→新名）

Google C++ スタイル（型は `PascalCase` で `C` 接頭辞なし、変数は `snake_case`、メンバは末尾 `_`、定数・enum 値は `kPascalCase`）に合わせて名前を付け替えた。**動作は変わらない。** 利用側のコードは、下の表の旧名を新名に置換すれば直る（公開メソッド名は元から `PascalCase` なので変更なし）。

### ファイル名

| 旧 | 新 |
|---|---|
| `Common.h` | `common.h` |
| `CommonUtil.h` / `CommonUtil.cpp` | `common_util.h` / `common_util.cc` |
| `CommonCtrl.h` / `CommonCtrl.cpp` | `common_ctrl.h` / `common_ctrl.cc` |
| `InputSimulator.h` / `InputSimulator.cpp` | `input_simulator.h` / `input_simulator.cc` |
| `FileComparer.h` / `FileComparer.cpp` | `file_comparer.h` / `file_comparer.cc` |
| `WorkerThreads.h` / `WorkerThreads.cpp` | `worker_threads.h` / `worker_threads.cc` |

利用側では `#include "Common.h"` 等の参照と、`.vcxproj` / `.vcxproj.filters` の `ClCompile` / `ClInclude` のパスを新しいファイル名に直す。

### クラス名・struct 名

| 旧 | 新 |
|---|---|
| `CBitmapStatic` | `BitmapStatic` |
| `CRestrictedEdit` | `RestrictedEdit` |
| `CPopupEdit` | `PopupEdit` |
| `CPopupList` | `PopupList` |
| `CEditableListCtrl` | `EditableListCtrl` |
| `CIconComboBox` | `IconComboBox` |
| `CSimpleListCtrl` | `SimpleListCtrl` |
| `CInputSimulator` | `InputSimulator` |
| `CFileComparer` | `FileComparer` |
| `CWorkerThreads` | `WorkerThreads` |
| `DLGITEMTEXT` | `DlgItemText` |
| `ICONCOMBOBOXITEM` | `IconComboBoxItem` |

`DECLARE_DYNAMIC` / `IMPLEMENT_DYNAMIC` / `BEGIN_MESSAGE_MAP` / `RUNTIME_CLASS` などのマクロ引数、派生クラスの基底クラス指定、`DDX_Control` の変数の型も新名にする。

### 定数

| 旧 | 新 |
|---|---|
| `VK_NONE` | `kVkNone` |
| `WM_POPUPEDIT_CLOSED` | `kWmPopupEditClosed` |
| `WM_POPUPLIST_CLOSED` | `kWmPopupListClosed` |
| `MAX_ICON_NUM` | `kMaxIconNum` |

### enum 値

| 旧 | 新 |
|---|---|
| `RestrictedEdit::CHAR_ANY` | `RestrictedEdit::kCharAny` |
| `RestrictedEdit::CHAR_DIGIT` | `RestrictedEdit::kCharDigit` |
| `RestrictedEdit::CHAR_DIGIT_SIGN` | `RestrictedEdit::kCharDigitSign` |
| `RestrictedEdit::CHAR_DECIMAL` | `RestrictedEdit::kCharDecimal` |
| `RestrictedEdit::CHAR_DECIMAL_SIGN` | `RestrictedEdit::kCharDecimalSign` |
| `RestrictedEdit::CHAR_ASCII` | `RestrictedEdit::kCharAscii` |
| `PopupEdit::INPUT_DIGIT` | `PopupEdit::kInputDigit` |
| `PopupEdit::INPUT_ALPHA` | `PopupEdit::kInputAlpha` |
| `PopupEdit::INPUT_DIGIT_ALPHA` | `PopupEdit::kInputDigitAlpha` |
| `PopupEdit::INPUT_ANY` | `PopupEdit::kInputAny` |
| `PopupEdit::RESULT_CANCEL` | `PopupEdit::kResultCancel` |
| `PopupEdit::RESULT_OK` | `PopupEdit::kResultOk` |
| `FileComparer::ERR_NONE` | `FileComparer::kErrNone` |
| `FileComparer::ERR_FILE1_OPEN` | `FileComparer::kErrFile1Open` |
| `FileComparer::ERR_FILE2_OPEN` | `FileComparer::kErrFile2Open` |

enum の型名（`CharKind` / `InputKind` / `Result` / `Error`）は元から `PascalCase` なので変更なし。

### struct のフィールド

| 旧 | 新 |
|---|---|
| `DLGITEMTEXT::ctrlId` | `DlgItemText::ctrl_id` |
| `DLGITEMTEXT::stringId` | `DlgItemText::string_id` |
| `ICONCOMBOBOXITEM::idText` | `IconComboBoxItem::text_id` |
| `ICONCOMBOBOXITEM::value` | `IconComboBoxItem::value`（変更なし） |
| `ICONCOMBOBOXITEM::dwConstraint` | `IconComboBoxItem::constraint` |
| `ICONCOMBOBOXITEM::idIcon[]` | `IconComboBoxItem::icon_ids[]` |
| `ICONCOMBOBOXITEM::idxItem` | `IconComboBoxItem::combo_index` |

### 派生クラスから見えるメンバ

| 旧 | 新 |
|---|---|
| `CEditableListCtrl::m_cursor`（protected） | `EditableListCtrl::cursor_` |

公開メソッド名（`SetListItems`、`SetEditKind`、`CreatePopup`、`UseInEditKey`、`MergePath`、`SplitPath` など）と、その引数の型・並びは変わらない（引数名だけ `snake_case` になった）。それ以外の `m_` メンバ・引数・ローカル変数は private か関数内なので、利用側からは見えない。
