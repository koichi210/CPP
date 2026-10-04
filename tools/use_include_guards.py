"""`#pragma once` を Google スタイルのインクルードガード(`<パス>_H_`)に置き換える。

ガード名はリポジトリ直下からのパスを大文字にして、英数字以外を `_` にしたもの
(例: othello/Source/board.h → OTHELLO_SOURCE_BOARD_H_)。
VS が生成するヘッダー(stdafx.h / pch.h / resource.h / targetver.h / framework.h)と、
外部ライブラリ(External / packages)は対象外。

    py -3 tools\\use_include_guards.py          # 書き換える
    py -3 tools\\use_include_guards.py --dry    # 対象を表示するだけ
"""
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SKIP_PATH = re.compile(r'/packages/|/External/|/GL/')
SKIP_NAME = re.compile(r'(?i)^(stdafx|pch|resource|targetver|framework)\.h$')
# 行末の \r は置換で消さない(先読みにする)
PRAGMA = re.compile(r'^[ \t]*#[ \t]*pragma[ \t]+once[ \t]*(?=\r?$)', re.M)


def guard_name(rel_path):
    name = re.sub(r'[^A-Za-z0-9]', '_', rel_path).upper()
    return name.lstrip('_') + '_'


def main():
    dry = '--dry' in sys.argv
    out = subprocess.run(['git', '-C', REPO, '-c', 'core.quotepath=false', 'ls-files', '-z', '*.h'],
                         capture_output=True, check=True).stdout
    changed = 0
    for rel in (n.decode('utf-8') for n in out.split(b'\0') if n):
        if SKIP_PATH.search(rel) or SKIP_NAME.match(os.path.basename(rel)):
            continue
        path = os.path.join(REPO, rel)
        data = open(path, 'rb').read()
        if data[:2] in (b'\xff\xfe', b'\xfe\xff'):
            continue
        bom = data.startswith(b'\xef\xbb\xbf')
        text = (data[3:] if bom else data).decode('utf-8')
        if not PRAGMA.search(text):
            continue
        nl = '\r\n' if '\r\n' in text else '\n'
        guard = guard_name(rel)
        if f'#ifndef {guard}' in text:
            continue
        text = PRAGMA.sub(f'#ifndef {guard}{nl}#define {guard}', text, count=1)
        if not text.endswith(nl):
            text += nl
        text = text.rstrip('\r\n') + nl + nl + f'#endif  // {guard}{nl}'
        changed += 1
        print(('DRY ' if dry else '') + f'{rel} -> {guard}')
        if not dry:
            open(path, 'wb').write((b'\xef\xbb\xbf' if bom else b'') + text.encode('utf-8'))
    print(f'changed={changed}')


main()
