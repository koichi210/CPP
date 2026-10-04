"""Google スタイルの命名規則(リポジトリ直下の .clang-tidy)に合わない名前を数える。

VS2019 に同梱の clang-tidy を使う。使い方:

    py -3 tools\\check_naming.py Clipboard\\Source          # 1つのプロジェクト
    py -3 tools\\check_naming.py --all                       # 全プロジェクト
    py -3 tools\\check_naming.py Clipboard\\Source --fix     # 見つけた違反を自動で書き換える

- clang は UTF-16 の resource.h を読めないため、実行中だけ UTF-8 にして、終わったら必ず戻す。
- 追加の include パスは -I<パス> で渡せる。_Common は自動で追加する。
- 違反がなければ終了コード 0。
- MFC のダイアログが必ず持つ `enum { IDD = ... }` は MFC が名前を決めているので、1件は出る。
"""
import glob
import os
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
VS_ROOT = os.environ.get('VS_ROOT', r'D:\Program Files (x86)\Microsoft Visual Studio\2019\Professional')
MSVC_VERSION = os.environ.get('MSVC_VERSION', '14.29.30133')
SDK_ROOT = os.environ.get('SDK_ROOT', r'C:\Program Files (x86)\Windows Kits\10\Include')
SDK_VERSION = os.environ.get('SDK_VERSION', '10.0.19041.0')

TIDY = os.path.join(VS_ROOT, r'VC\Tools\Llvm\x64\bin\clang-tidy.exe')
MSVC = os.path.join(VS_ROOT, 'VC', 'Tools', 'MSVC', MSVC_VERSION).replace('\\', '/')
SDK = os.path.join(SDK_ROOT, SDK_VERSION).replace('\\', '/')

SKIP_DIRS = ('packages', '.git', '.claude', '.vs')


def project_dirs():
    """vcxproj があるフォルダ(=ソースのフォルダ)を全部返す。"""
    found = []
    for root, dirs, files in os.walk(REPO):
        dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
        if any(f.endswith('.vcxproj') for f in files):
            found.append(root)
    return sorted(found)


def display_name(path):
    try:
        return os.path.relpath(path, REPO)
    except ValueError:  # 別ドライブのときは絶対パスのまま
        return path


def check_dir(src_dir, extra_includes, fix):
    cpps = [p for p in glob.glob(os.path.join(src_dir, '*.cc')) + glob.glob(os.path.join(src_dir, '*.cpp'))
            if os.path.basename(p).lower() not in ('stdafx.cpp', 'pch.cpp')]
    if not cpps:
        return {}, []

    backups = {}
    for h in glob.glob(os.path.join(src_dir, '[Rr]esource.h')):
        data = open(h, 'rb').read()
        if data[:2] in (b'\xff\xfe', b'\xfe\xff'):
            backups[h] = data
            open(h, 'wb').write(data.decode('utf-16').encode('utf-8'))

    # OpenGL のように NuGet の include を使うプロジェクト
    nuget = glob.glob(os.path.join(os.path.dirname(src_dir), 'packages', '*', 'build', 'native', 'include'))

    args = ['-quiet', '-header-filter=' + src_dir.replace('\\', '/') + '/.*']
    if fix:
        args.append('-fix')
    compiler = ['--', '--driver-mode=cl', '-m32', '/std:c++17', '-fms-compatibility', '-fms-extensions',
                '-fms-compatibility-version=19.29', '-DWIN32', '-D_WINDOWS', '-DNDEBUG', '-D_MBCS',
                '-imsvc', MSVC + '/include', '-imsvc', MSVC + '/atlmfc/include',
                '-imsvc', SDK + '/ucrt', '-imsvc', SDK + '/um', '-imsvc', SDK + '/shared', '/utf-8',
                '-I' + src_dir, '-I' + os.path.join(REPO, '_Common')]
    compiler += ['-I' + n for n in nuget] + ['-I' + i for i in extra_includes]

    violations = {}
    parse_errors = []
    try:
        for cpp in cpps:
            out = subprocess.run([TIDY, cpp] + args + compiler, capture_output=True, text=True,
                                 encoding='utf-8', errors='replace').stdout
            for line in out.splitlines():
                if 'readability-identifier-naming' in line and 'warning:' in line:
                    key = line.split(': warning: ', 1)[1]
                    violations[key] = violations.get(key, 0) + 1
                elif ': error:' in line and 'clang-diagnostic' in line:
                    parse_errors.append(line[:200])
    finally:
        for h, data in backups.items():
            open(h, 'wb').write(data)
    return violations, parse_errors


def main():
    argv = sys.argv[1:]
    fix = '--fix' in argv
    extra = [a[2:] for a in argv if a.startswith('-I')]
    dirs = [a for a in argv if not a.startswith('-')]
    if '--all' in argv:
        dirs = project_dirs()
    if not dirs:
        print(__doc__)
        return 2

    total = 0
    for d in dirs:
        d = os.path.abspath(d)
        if os.path.commonpath([d, REPO]) != REPO:
            print(f'エラー: {d} はリポジトリの外です。.clang-tidy が見つからず、何も検査されません。')
            return 2
        violations, parse_errors = check_dir(d, extra, fix)
        count = sum(violations.values())
        total += count
        print(f'{display_name(d)}: violations={count}' + (f' parse-errors={len(parse_errors)}' if parse_errors else ''))
        for key, n in sorted(violations.items()):
            print(f'    {n:3d}  {key[:140]}')
    print(f'total violations={total}')
    return 0 if total == 0 else 1


sys.exit(main())
