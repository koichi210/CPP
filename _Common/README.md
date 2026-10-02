# _Common

MFC（MBCS / Shift-JIS）アプリ向けの共通部品。各プロジェクトにコピーせず、ここを参照して使う。

| ファイル | 内容 |
|---|---|
| `Common.h` | 下の全部をまとめてインクルード |
| `CommonUtil` | パス操作・文字列置換・半角⇔全角・フォルダ選択・ダイアログ文字列設定 |
| `CommonCtrl` | `CBitmapStatic` / `CRestrictedEdit` / `CPopupEdit` / `CPopupList` / `CEditableListCtrl` / `CIconComboBox` / `CSimpleListCtrl` |
| `InputSimulator` | キーボード・マウス入力のエミュレート |
| `FileComparer` | 2ファイルのバイナリ比較 |

## プロジェクトへの組み込み

1. 使う `.cpp` をプロジェクトに追加する（例: `..\..\_Common\CommonUtil.cpp`）
2. 追加した `_Common` の `.cpp` は **プリコンパイル済みヘッダーを「使用しない」** にする
   （プロジェクトごとに `stdafx.h` / `pch.h` が違うため、`_Common` 側は PCH に依存しない作り）
3. インクルードディレクトリに `_Common` を追加し、`#include "Common.h"`
4. 文字セットは「マルチバイト」。ソースは UTF-8(BOM付) なので、
   C/C++ の追加オプションに `/source-charset:utf-8 /execution-charset:.932` を指定する
   （`CommonUtil.cpp` の `static_assert` で Shift-JIS 埋め込みを確認している）

## 旧 StandardTemplate からの移行表

| 旧 | 新 | 備考 |
|---|---|---|
| `MyMergePath(CString*, ...)` | `MergePath(CString&, ...)` | 区切りの `\` を重複させない |
| `MySplitPath(...)` | `SplitPath(...)` | 空の要素も出力に反映する（以前は前の値が残った） |
| `MyConnection` | `AppendPath` に統合 | |
| `AppendPath(CString*, CString)` | `AppendPath(CString&, LPCTSTR)` | 空のパスに連結しても先頭に `\` を付けない |
| `AppendExt(CString*, CString)` | `AppendExt(CString&, LPCTSTR)` | `".txt"` を渡しても `..txt` にならない |
| `ReplaceString(old, CString* out, srch, rep, bDiff)` | `CString ReplaceString(src, search, replace, bCaseSensitive)` | 戻り値で返す。TRUE で大文字小文字を区別（旧コメントは逆に書かれていた） |
| `Browse(hWnd, title, CString*)` | `BrowseFolder(hWnd, title, CString&)` | |
| `TABLE` / `SetDlgItemTextAll` | `DLGITEMTEXT` / `SetDlgItemTextAll` | |
| `han2zen(char*)` / `zen2han(char*)` | `CString HankakuToZenkaku(CString)` / `CString ZenkakuToHankaku(CString)` | 戻り値で返す（バッファ長の心配が不要） |
| `issjiskanji(c)` | `IsSjisLeadByte(c)` | |
| `CMyStaticImage` | `CBitmapStatic` | |
| `CMyRistrictedSBCS` | `CRestrictedEdit` | `F_DIGIT` 等 → `CHAR_DIGIT` 等、`SetExceptionCharacters` → `SetForbiddenCharacters` |
| `CMyEdit` | `CPopupEdit` | `USESTRING_*` → `INPUT_*`、`STATE_OK/NG` → `RESULT_OK/CANCEL` |
| `CMyList` | `CPopupList` | |
| `CMyListCtrl` | `CEditableListCtrl` | `SetElements` → `SetListItems`、`SetEditPossible` → `SetEditKind`。仮想関数 `CreatePopup` / `UseInEditKey` は名前そのまま |
| `WM_MYEDIT_KILLFOCUS` / `WM_MYLIST_KILLFOCUS` | `WM_POPUPEDIT_CLOSED` / `WM_POPUPLIST_CLOSED` | |
| `CMyEvent` | `CInputSimulator` | メンバは static になった（インスタンス経由の呼び出しもそのまま動く） |
| `CCustomCtrl` | `CSimpleListCtrl` | `SetLabel` → `AddColumn`、`SetMaxColumnNum` → `SetRowCount`、`FillRect` → `FillColumn`、`SelectIdx` → `SelectRow`、`EnableSelectRowAll` → `EnableFullRowSelect`、`SetMaxRowNum` は未使用のため削除 |
| `CMyCompareFile` | `CFileComparer` | `m_err` → `GetError()` |
| `Trim` / `GetArgument` / `GetArgumentSample` | 削除 | 未使用。`Trim` は `CString::Trim(LPCTSTR)` で代替可 |
| `USE_IMM_H`（IME 無効化） | 削除 | 既定で無効のまま、有効化しているプロジェクトが無かった |
