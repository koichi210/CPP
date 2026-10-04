# CPP

C++ の学習・検証コード置き場。MFC のダイアログアプリが中心で、1フォルダが1プロジェクト（`.sln` を持つ）。

## ビルド環境

- Visual Studio 2019（ツールセット v142、C++17）。「C++ によるデスクトップ開発」と「最新の v142 ビルドツール用 C++ MFC (x86 および x64)」が必要
- 文字セットは「マルチバイト」のプロジェクトが多い（Shift-JIS で文字列を埋め込む）。ソースは UTF-8（BOM 付き）で、
  プロジェクトに `/source-charset:utf-8 /execution-charset:.932` を指定している
- リソース（`.rc`）と `resource.h` は VS が決める文字コード（UTF-16 など）のまま触らない

```
MSBuild.exe <プロジェクト>\<プロジェクト>.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64
```

- プラットフォームは x64 だけ（x86 / Win32 の構成は 2026-10 に削除した）。exe は各プロジェクトの `Release\` に出る

## 共通部品 `_Common`

複数のプロジェクトが共有する MFC 部品（パス操作、リストコントロール、入力のエミュレート、ファイル比較、
ワーカースレッドの終了待ちなど）。使い方と旧名からの移行表は [`_Common/README.md`](_Common/README.md)。

## 命名規則

[Google C++ スタイル](https://google.github.io/styleguide/cppguide.html)に合わせる。

| 対象 | 規則 | 例 |
|---|---|---|
| ファイル | 小文字の `snake_case`、拡張子は `.cc` と `.h` | `clipboard_dlg.cc` |
| 型（class / struct / enum / using） | `PascalCase`、`C` 接頭辞は付けない | `ClipboardDlg` |
| 関数・メソッド | `PascalCase` | `OnBnClickedStart()` |
| 変数・引数・ローカル | `snake_case`、ハンガリアン記法は使わない | `parent`、`icon_width` |
| クラスのデータメンバ | `snake_case` ＋ 末尾 `_` | `frame_rate_` |
| struct のデータメンバ（データだけの入れ物） | `snake_case`（末尾 `_` なし） | `old_path` |
| 定数・enum の値 | `kPascalCase` | `kBoardSize`、`Stone::kBlack` |
| マクロ | `ALL_CAPS` | `STR_BUFF` |
| 名前空間 | 小文字 | `greeting` |

次のものは MFC / Win32 / Visual Studio が名前を決めているので、そのままにする。

- 基底クラスのメンバ（`m_hWnd`、`m_pMainWnd` など）と、オーバーライドする関数・メッセージハンドラの名前（`OnInitDialog`、`DoDataExchange` など）
- ダイアログの `enum { IDD = ... }`、リソース ID（`IDC_*` など）
- VS が作るファイル名（`stdafx.h`、`pch.h`、`resource.h`、`targetver.h`、`*.rc`）
- プロジェクト名、exe 名

### 命名の検査

VS2019 に同梱の clang-tidy で、規則に合わない名前を数える（設定はリポジトリ直下の [`.clang-tidy`](.clang-tidy)）。

```
py -3 tools\check_naming.py Clipboard\Source        # 1つのプロジェクト
py -3 tools\check_naming.py --all                    # 全プロジェクト
py -3 tools\check_naming.py Clipboard\Source --fix   # 大文字小文字の違いなど、機械的に直せるものを書き換える
```

`C` 接頭辞のクラスの検出と、ファイル名の確認はこのツールではできないので、目で確認する。

## コミットメッセージ

`<プロジェクト名>: <変更の要約>` の形で、日本語で書く。
